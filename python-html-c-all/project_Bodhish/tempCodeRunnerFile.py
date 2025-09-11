# app.py
from flask import Flask, request, jsonify, send_from_directory
from google.adk.agents import LlmAgent
from google.adk.runners import Runner
from google.adk.sessions import InMemorySessionService
from google.genai import types
import logging
import traceback

# Define the "tool" function that sends coordinates (in this context, returns JSON)
def send_coords_tool(target: str) -> dict:
    """Tool: given a target, return the coordinates to send to Arduino."""
    mapping = {
        "plant1": {"x": 800, "y": 800, "z": 1000},
        "plant2": {"x": 2400, "y": 1200, "z": 1000},
        "home": {"x": 0, "y": 0, "z": 0},
    }
    coords = mapping.get(target.lower())
    if not coords:
        return {"error": f"Unknown target: {target}"}
    return {"action": "sendCoords", "target": target.lower(), **coords}

# Configure the agent with the tool
agent = LlmAgent(
    name="plant_watering_agent",
    model="gemini-2.5-flash-preview-05-20",
    tools=[send_coords_tool],
    instruction="""
You are an agent controlling a plant-watering robot. When user gives a command 
like 'water plant 2' or 'go home', extract the target (plant1, plant2, or home), 
then use the send_coords_tool(target) function. Respond with the function call only 
(including JSON payload). Do NOT provide any other text.
"""
)

# Set up runner and in-memory session service
session_service = InMemorySessionService()
APP_NAME = "krishibodh_app"
USER_ID = "user_1"
SESSION_ID = "session_1"
session = session_service.create_session(
    app_name=APP_NAME, user_id=USER_ID, session_id=SESSION_ID)
runner = Runner(agent=agent, app_name=APP_NAME, session_service=session_service)

# Configure logging
logging.basicConfig(level=logging.DEBUG)
logger = logging.getLogger(__name__)

# Flask API
app = Flask(__name__, static_url_path='')

@app.route("/ask_agent", methods=["POST"])
def ask_agent():
    try:
        user_prompt = request.json.get("prompt", "")
        if not user_prompt:
            return jsonify({"error": "No prompt provided"}), 400

        logger.debug(f"Received prompt: {user_prompt}")
        
        content = types.Content(role="user", parts=[types.Part.from_text(text=user_prompt)])
        events = runner.run(user_id=USER_ID, session_id=SESSION_ID, new_message=content)

        # Look for tool call or final response
        for ev in events:
            if ev.actions.function_calls:
                # Agent is requesting to call our tool
                for call in ev.actions.function_calls:
                    if call.name == "send_coords_tool":
                        args = call.args
                        result = send_coords_tool(args.get("target", ""))
                        return jsonify(result)
            if ev.is_final_response():
                # If no tool call, return agent text
                return jsonify({"response": ev.content.parts[0].text})

        return jsonify({"error": "No response from agent."}), 500

    except Exception as e:
        logger.error(f"Error processing request: {str(e)}")
        logger.error(traceback.format_exc())
        return jsonify({"error": f"Internal server error: {str(e)}"}), 500

@app.route('/')
def index():
    return send_from_directory('.', 'index.html')

@app.route('/style.css')
def style():
    return send_from_directory('.', 'style.css')

if __name__ == "__main__":
    app.run(port=5000, debug=True)
