# toyLab
## 高级OpenGL
1. #### 模板测试
* 当你开启模板测试时，画场景时其实是同时画了color buffer,depth buffer和stencil buffer
2.  #### 深度测试和混合
*  先绘制所有不透明的物体
* 对所有透明的物体排序。
* 按顺序绘制所有透明的物体。
* 更高级的技术还有次序无关透明度(Order Independent Transparency, OIT)
* **最终颜色**=(目标颜色*目标因子)@(源颜色*源因子)