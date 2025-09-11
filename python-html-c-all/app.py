from flask import Flask, request, jsonify
from google import genai
from google.genai import types

app = Flask(__name__)

genai.configure(api_key="AIzaSyA-dl4hUTa9-z-nBVlB4aGzC0rZkigottw")

model = genai.GenerativeModel(
    model_name="gemini-2.5-flash-preview-05-20",
    generation_config={"temperature": 0.2},
    system_instruction="""You are an assistant for a plant-watering robot. Given any user command (natural language), return a clean JSON with the following format and nothing else:

{
  "action": "sendCoords",
  "target": "plant1" | "plant2" | "home",
  "x": <int>,
  "y": <int>,
  "z": <int>
}

Supported targets:
- plant1: x=800, y=800, z=1000  
- plant2: x=2400, y=1200, z=1000  
- home: x=0, y=0, z=0  

Return JSON only. No markdown, no explanation, no extra text."""
)

@app.route("/ask", methods=["POST"])
def ask():
    prompt = request.json.get("prompt", "")
    if not prompt:
        return jsonify({"error": "Empty prompt"}), 400

    response = model.generate_content(prompt)
    text = response.text.strip()

    try:
        json_start = text.find("{")
        json_data = text[json_start:]
        result = eval(json_data)  # Only safe if AI is strictly returning JSON!
        return jsonify(result)
    except Exception as e:
        return jsonify({"error": "Failed to parse AI response", "raw": text}), 500

if __name__ == "__main__":
    app.run(debug=True)
