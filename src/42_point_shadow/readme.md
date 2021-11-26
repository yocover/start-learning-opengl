## 点光源阴影

### 立方体阴影

![image-20211126160855750](images/image-20211126160855750.png)

> 盒子内部需要法线反转才能正确显示阴影贴图的信息
>
> ```glsl
> vs_out.Normal = transpose(inverse(mat3(model))) * (reverse_normal * normal);
> ```

### PCF边缘柔化

![image-20211126164416112](images/image-20211126164416112.png)

## 参考
