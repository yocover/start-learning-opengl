## 镜面反射 IBL



![image-20211224155928576](images/image-20211224155928576.png)

- 第一部分：预先计算的环境卷积贴图
- 第二部分：BRDF积分贴图，存储菲涅尔响应系数

**预滤波环境贴图**

![image-20211224160828377](images/image-20211224160828377.png)

**BRDF积分贴图**

存储菲涅尔以下系数，

- R：响应系数
- G：偏差值

![image-20211224160934384](images/image-20211224160934384.png)

**将预过滤贴图和BRDF组合获取IBL镜面反射**

![image-20211224161347300](images/image-20211224161347300.png)

![image-20211224161421597](images/image-20211224161421597.png)

## 参考

https://learnopengl-cn.github.io/07%20PBR/03%20IBL/02%20Specular%20IBL/#ibl
