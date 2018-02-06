# Image-in-painting
这是一个图像修补、去水印的测试程序，不是正式的产品。
目前实现了以下功能：
1. 对分辨率相同的批量图片，自动检测水印。如果大多数图片在相同的位置上有相同或近似的水印，那么就能检测出来。计划用于视频去水印的自动化操作。
2. 手工标识水印或要修补的区域，可以使用矩形标注、闭合路径（多边形）标注、画笔标注。可以撤消和重做标记操作。
3. 三种方式去除水印或修补图像。
    a. 使用标记的修补区域边缘像素在x和y方向进行平滑过渡，速度很快。
    b. 使用标记的修补区域边缘像素从外向内生长，速度较快。
    c. 在一定范围寻找可以用于修补的小图块，拼接到待修补的区域，速度很慢。
4. 界面程序实现了图像浏览、放大缩小、以及使用鼠标进行标记操作的功能。

文件夹说明：
Gui : Qt 5.9.2 (VC2015) 编写的工具界面。
Inpaint : 实现修补算法和接口的代码，可以脱离 Qt 的环境加入任意 C++ 的项目中使用。
