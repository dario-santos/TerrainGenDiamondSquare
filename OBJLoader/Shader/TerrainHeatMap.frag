#version 330 core

out vec4 FragColor;

in vec3 FragPos;

uniform vec3 objectColor;

void main()
{
  vec3 c = FragPos;
    
  // y <= 0           -> sea level
  // y > 0 && y <= 3  -> sand
  // y > 3 && y <= 8  -> grass
  // y > 8 && y <= 40 -> mountains
  // else             -> sown

  if(FragPos.y <= 0)
    c = vec3(12,84,92) / 255.0f;
  else if(FragPos.y > 0 && FragPos.y <= 3)
    c = vec3(143,126,95) / 255.0f;
  else if(FragPos.y > 3 && FragPos.y <= 8)
    c = vec3(106,127,63) / 255.0f;
  else if(FragPos.y > 8 && FragPos.y <= 40)
    c = vec3(102,108,111) / 255.0f;
  else
    c = vec3(195,199,218) / 255.0f;

  FragColor = vec4(c, 1.0);
}
