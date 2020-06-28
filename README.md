# ToyLab
### 1. 光照
#### 1.1 基础光照
* **法线矩阵**被定义为「模型矩阵左上角的逆矩阵的转置矩阵」。注意，大部分的资源都会将法线矩阵定义为应用到模型-观察矩阵(Model-view Matrix)上的操作，但是由于我们只在世界空间中进行操作（不是在观察空间），我们只使用模型矩阵。

* 在顶点着色器中，我们可以使用inverse和transpose函数自己生成这个法线矩阵，这两个函数对所有类型矩阵都有效。注意我们还要把被处理过的矩阵强制转换为3×3矩阵，来保证它失去了位移属性以及能够乘以vec3的法向量。
```
Normal = mat3(transpose(inverse(model))) * aNormal;
```


### 2.  高级OpenGL
####  2.1   深度测试
* **提前深度测试**：当使用提前深度测试时，片段着色器的一个限制是你不能写入片段的深度值。如果一个片段着色器对它的深度值进行了写入，提前深度测试是不可能的。OpenGL不能提前知道深度值。

![depth_mapping](/resources/document/depth_non_linear_graph.png)

#### 2.2    模板测试
* 当你开启模板测试时，画场景时其实是同时画了color buffer,depth buffer和stencil buffer
#### 2.3    深度测试和混合
* **提前深度测试**:  Early-Z的实现，主要是通过一个Z-pre-pass实现。简单来说：对不透明物体
    * a. 首先用简单shader进行渲染，这个shader不写颜色缓冲区，只写深度缓冲区。
    * b. 关闭深度写入，开启深度测试，用正常的shader进行渲染。
    * c. The most effective way to use early depth test hardware is to run a depth-only pre-processing pass. This means to render all available geometry, using minimal shaders and a rendering pipeline that only writes to the depth buffer. The Vertex Shader should do nothing more than transform positions, and the Fragment Shader does not even need to exist.
    * d. [earlyZ链接](https://liangz0707.github.io/whoimi/blogs/RTR/EarlyZ.html)

This provides the best performance gain if the fragment shader is expensive, or if you intend to use multiple passes across the geometry.
*  先绘制所有不透明的物体
* 对所有透明的物体排序。
* 按顺序绘制所有透明的物体。
* 更高级的技术还有次序无关透明度(Order Independent Transparency, OIT)
* **最终颜色**=(目标颜色*目标因子)@(源颜色*源因子)
#### 2.4    帧缓冲
* 附加至少一个缓冲（颜色、深度或模板缓冲）。
* 至少有一个颜色附件(Attachment)。
    * 附件:  纹理或渲染缓冲对象(Renderbuffer Object)。
    * 纹理： 
    * 渲染缓冲对象：渲染缓冲对象(Renderbuffer Object)是在纹理之后引入到OpenGL中， 好处是，它会将数据储存为OpenGL原生的渲染格式，它是为离屏渲染到帧缓冲优化过的， 所以，交换缓冲这样的操作在使用渲染缓冲对象时会非常快。
* 所有的附件都必须是完整的（保留了内存）。
* 每个缓冲都应该有相同的样本数。

#### 2.5 高级数据
* **OpenGL中的缓冲** : 只是一个管理特定内存块的对象，没有其它更多的功能了。在我们将它绑定到一个缓冲目标(Buffer Target)时，我们才赋予了其意义。opengl内部每个目标绑定一个缓冲， 根据目标的不同以不同的方式来处理缓冲。
* 复制缓冲： ``void glCopyBufferSubData(GLenum readtarget, GLenum writetarget, GLintptr readoffset, GLintptr writeoffset, GLsizeiptr size); ``
* 对一个缓冲调用glBufferSubData之前必须要先调用glBufferData `` void glBufferSubData(	GLenum target,GLintptr offset, GLsizeiptr size, const void * data);``

#### 2.6 几何着色器
* 几何着色器的输入是一个图元（如点或三角形）的一组顶点。几何着色器可以在顶点发送到下一着色器阶段之前对它们随意变换。
然而，几何着色器最有趣的地方在于，它能够将（这一组）顶点变换为完全不同的图元，并且还能生成比原来更多的顶点。

#### 2.7 实例化
* **实例化** : 够将数据一次性发送给GPU，然后使用一个绘制函数让OpenGL利用这些数据绘制多个物体。实例化这项技术能够让我们使用一个渲染调用来绘制多个物体，来节省每次绘制物体时CPU -> GPU的通信，它只需要一次即可。如果想使用实例化渲染，我们只需要将glDrawArrays和glDrawElements的渲染调用分别改为glDrawArraysInstanced和glDrawElementsInstanced就可以了。这些渲染函数的实例化版本需要一个额外的参数，叫做实例数量(Instance Count)，它能够设置我们需要渲染的实例个数。这样我们只需要将必须的数据发送到GPU一次，然后使用一次函数调用告诉GPU它应该如何绘制这些实例。GPU将会直接渲染这些实例，而不用不断地与CPU进行通信

