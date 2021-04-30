## 纹理 练习

- 修改笑脸朝向

  ```glsl
  // fragment glsl
  FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(-TexCoord.x, TextCoord.y)), 0.2);
  ```
  
- 尝试不同的环绕方式

- `GL_CLAMP_TO_BORDER`环绕方式

  ![image-20210421215156603](images/image-20210421215156603.png)

- `GL_CLAMP_TO_EDGE`环绕方式

  ![image-20210421215241280](images/image-20210421215241280.png)

- `GL_MIRRORED_REPEAT`环绕方式

  ![image-20210421215319864](images/image-20210421215319864.png)

- `GL_REPEAT`环绕方式

  ![image-20210421215400944](images/image-20210421215400944.png)

- 使用`GL_NEAREST`过滤方式

  ![image-20210421214856449](images/image-20210421214856449.png)

- 使用`GL_LINEAR`过滤方式

  ![image-20210421215031627](images/image-20210421215031627.png)

- 使用一个`uniform`变量作为`mix`函数的第三个参数来改变两个纹理可见度

  ```glsl
  // fragment glsl
  FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), abs(sin(factor * 0.2)));
  ```

  