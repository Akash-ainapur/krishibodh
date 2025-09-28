import os
import json
from flask import Flask, request, jsonify, send_from_directory
import google.generativeai as genai

# --- Flask App Setup ---
app = Flask(__name__)


# --- Gemini API Configuration ---
API_KEY = "AIzaSyDhJL7mLlFmwNQv-RHO0yyIxfPoIPyA8jU"  # Your API key

# Configure the API
genai.configure(api_key=API_KEY)

# --- System Instruction for the AI Model ---
# This prompt contains all the logic that was previously in the "tool" function.
# It instructs the model on how to behave and what format to return.
SYSTEM_INSTRUCTION = """
You are a helpful assistant that controls a plant-watering robot.
Based on the user's command, you will determine the target location.
The supported targets and their coordinates are:
- "plant1": {"x": 800, "y": 800, "z": 25000}
- "plant2": {"x": 2400, "y": 1200, "z": 25000}
- "home": {"x": 0, "y": 0, "z": 0}

Your only job is to respond with a clean JSON object in the following format and nothing else.
Do not add any explanation, markdown formatting, or any text outside of the JSON structure.

{
  "action": "sendCoords",
  "target": "<target_name>",
  "x": <x_coordinate>,
  "y": <y_coordinate>,
  "z": <z_coordinate>
}
"""

# --- Initialize the Generative Model ---
model = genai.GenerativeModel(
    model_name="gemini-1.5-flash", # Using a stable and fast model
    system_instruction=SYSTEM_INSTRUCTION,
    generation_config={
        "temperature": 0.0, # Set to 0 for deterministic, consistent JSON output
        "response_mime_type": "application/json", # Ensures the model outputs valid JSON
    }
)


# --- API Endpoint ---
@app.route("/api/ask", methods=["POST"])
def ask():
    """
    Receives a user prompt and returns a JSON command for the robot.
    """
    # Check if the API key was successfully configured at startup
    if not API_KEY:
         return jsonify({"error": "API key is not configured on the server. Please check the terminal logs."}), 500

    prompt = request.json.get("prompt", "")
    if not prompt:
        return jsonify({"error": "Prompt cannot be empty."}), 400

    try:
        # Generate content using the model
        response = model.generate_content(prompt)
        
        # The model is configured to return JSON, so we can parse it directly.
        # Using json.loads is safer than eval().
        result = json.loads(response.text)
        
        return jsonify(result)

    except json.JSONDecodeError:
        # This handles cases where the model unexpectedly returns non-JSON text
        return jsonify({
            "error": "Failed to parse the AI's response as JSON.",
            "raw_response": response.text
        }), 500
    except Exception as e:
        # Catch any other potential errors during the API call
        print(f"An unexpected error occurred: {e}")
        return jsonify({"error": f"An internal server error occurred: {str(e)}"}), 500

# --- Static File Serving for a Simple Frontend ---
@app.route('/')
def index():
    return send_from_directory('.', 'index.html')


# --- Main execution ---
if __name__ == "__main__":
    # Note: debug=True is for development. Remove it for production.
    app.run(port=5000, debug=True)
