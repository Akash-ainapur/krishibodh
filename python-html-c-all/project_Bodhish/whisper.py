# To run this code you need to install the following dependencies:
# pip install google-genai

import base64
import os
from google import genai
from google.genai import types


def generate():
    client = genai.Client(
        api_key="AIzaSyA-dl4hUTa9-z-nBVlB4aGzC0rZkigottw",
    )

    model = "gemini-2.5-flash-preview-05-20"
    contents = [
        types.Content(
            role="user",
            parts=[
                types.Part.from_text(text="""bodhish water plant 2 now immidiately
"""),
            ],
        ),
        types.Content(
            role="model",
            parts=[
                types.Part.from_text(text="""**Determining Target Coordinates**

I'm currently working on determining the coordinates for \"plant 2\" from the known target data. Once I've pinpointed the location, I'll incorporate the \"water\" action into the command. I'm focusing on efficiency and accuracy, ensuring the right plant gets the right action.


"""),
                types.Part.from_text(text="""```json
{
  \"action\": \"water\",
  \"target\": \"plant2\",
  \"coordinates\": {
    \"x\": 2400,
    \"y\": 1200,
    \"z\": 1000
  }
}
```"""),
            ],
        ),
        types.Content(
            role="user",
            parts=[
                types.Part.from_text(text="""bodhish water plant 1 now please i am lazy i cant do it now
"""),
            ],
        ),
        types.Content(
            role="model",
            parts=[
                types.Part.from_text(text="""**Pinpointing the Action**

I've identified the core action as \"water\" and the target as \"plant1.\" My next step is to efficiently retrieve the coordinates associated with \"plant1\" from my known target database. The goal is to perform the task with precision.


"""),
                types.Part.from_text(text="""```json
{
  \"action\": \"water\",
  \"target\": \"plant1\",
  \"coordinates\": {
    \"x\": 800,
    \"y\": 800,
    \"z\": 1000
  }
}
```"""),
            ],
        ),
        types.Content(
            role="user",
            parts=[
                types.Part.from_text(text="""water plant 2 bodhish"""),
            ],
        ),
    ]
    generate_content_config = types.GenerateContentConfig(
    temperature=0.2,
    response_mime_type="text/plain",
    system_instruction=[
        types.Part.from_text(text="""You are an assistant for a plant-watering robot. Given any user command (natural language), return a clean JSON with the following format and nothing else:

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

Return JSON only. No markdown, no explanation, no extra text."""),
    ],
)


    for chunk in client.models.generate_content_stream(
        model=model,
        contents=contents,
        config=generate_content_config,
    ):
        print(chunk.text, end="")

if __name__ == "__main__":
    generate()
