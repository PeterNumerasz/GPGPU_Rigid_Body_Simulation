#include "Texture.h"

#include "stb_image/stb_image.h"

Texture::Texture(const std::string& path)
    : m_RendererID(0),
      m_FilePath(path),
      m_LocalBuffer(nullptr),
      m_Width(0),
      m_Height(0),
      m_BPP(0) {
  // Flips the texture upside down, reason is OpenGl expects our
  // texture pixels to start from the bottom left
  stbi_set_flip_vertically_on_load(1);

  // Parameters: 1. - The path to the texture
  //             2. - X, stbi library will write it into our m_Width variable
  //             3. - Y, stbi library will write it into our m_Height variable
  //             4. - channels in file, will be written into m_BPP variable
  //             5. - desired channels, in this case (PNG) it's 4
  m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

  GLCall(glGenTextures(1, &m_RendererID));
  GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

  // These four parameters must be defined otherwise (depending on your OpenGL implementation) 
  // you might get a black texture
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));  // minification filter, for downscaling
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));  // magnification filter, for upscaling
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));  // horizontal clamping
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));  // vertical clamping

  GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
  GLCall(glBindTexture(GL_TEXTURE_2D, 0));

  if (m_LocalBuffer) stbi_image_free(m_LocalBuffer);
}

Texture::~Texture() { GLCall(glDeleteTextures(1, &m_RendererID)); }

void Texture::Bind(unsigned int slot /*= 0*/) const {
  GLCall(glActiveTexture(GL_TEXTURE0 + slot));
  GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const { GLCall(glBindTexture(GL_TEXTURE_2D, 0)); }
