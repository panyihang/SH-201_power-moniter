# SH-201_智能太阳能功率表

###

### 3.2寸大屏的电压电流表

<span class="colour" style="color: rgb(230, 126, 35);">Q群 565264047 欢迎来玩～</span>

# <span class="colour" style="color: rgb(45, 194, 107);">如需合作请联系邮箱 <u>[root@an-ye.top](mailto:root@an-ye.top)</u></span>

![](https://image.lceda.cn/pullimage/arQxZXJ8Mlkj4D3LzyLkc0r4rlcc5lX3y8VFWFZ8.jpeg)

![](https://image.lceda.cn/pullimage/jqNtsTtEuF7kAE2ffq0PEWK6AMvOLgZxMCirbMPY.jpeg)

![](https://image.lceda.cn/pullimage/xx6mCaEcfP3GcnY5op4DNtoQttrHJuXhFU4zxhPt.jpeg)
烧录需使用「[Cube-101_esp-tool烧录调试工具](/an_ye/cube-tong-yongesp-shao-lu-mu-kuai)」，正向插入TYPE-C口，如上图

代码开源仓库：[https://github.com/panyihang/SH-201_power-moniter](https://github.com/panyihang/SH-201_power-moniter)
正在更新，代码还不完善，但。。。又不是不能用（欢迎各位巨佬提pr

#

# 0x00:前言

B站「低成本家庭发电站」系列正式开始填坑，原谅已经咕咕咕了快一年

# 0x01:更新说明

<span class="colour" style="color: rgb(185, 106, 217);">「2022.06.23 Ver0.2」修改LED供电，原方案电流不足，调整走线，修改RP2040封装</span>
<span class="colour" style="color: rgb(53, 152, 219);">「2022.07.08 Ver0.3」更换MCU，使用ESP32S2</span>

# 0x02:硬件部分

* 主控MCU：ESP32S2
* INA199电流传感器
* AHT21温湿度传感器
* TFT8K2798屏幕，显示效果可以
* 背部磁吸，方便固定墙上（需要墙面支持）ps:磁吸墙面教程在后面后面

# 0x03:软件部分

* 系统：Arch linux
* 开发环境：Esp-idf 4.3
* 显示框架：LVGL7.2 <span class="colour" style="color: rgb(224, 62, 45);">（魔改过底层函数，不通用）</span>
* <span class="colour" style="color: rgb(0, 0, 0);"><span class="colour" style="color: rgb(0, 0, 0);">基于乐鑫官方移植项目lv\_port\_esp32</span></span>

# 0x04:编译/开发/烧录说明

编译&烧录命令：
开发环境esp-idf 4.3，Arch linux

`cd xxx(工程目录下）`
`. ~/esp/esp-idf-4.3/export.sh `
`idf.py build`
(这时插上板子）
`idf.py flash`

附带使用esptool.py烧录二进制文件的说明
烧录命令：
`(PORT)`   取决于操作系统和烧录方式，例如:

|  |  |  |
| --- | --- | --- |
| / | 手动USB烧录 | [Cube-esp-tool](/an_ye/cube-tong-yongesp-shao-lu-mu-kuai)快速烧录 |
| Linux | /dev/ttyACM[X] | /dev/ttyUSB[X] |
| Windows | COM[X] | COM[X] |

#

# 0x05:如何造一个支持磁吸的墙面（超低成本版）

Fe3O4粉磁吸区域是黑灰色的，无法改色
Fe粉看起来只是偏黄一点，基本能看出原色，但南方潮湿不建议使用

首先，准备任意颜色的内墙乳胶漆100g（1000g  8元左右，别买多了，用不完的。。。）
![](https://image.lceda.cn/pullimage/EJYhgZJ6EyzWkFq1c043sOPVCr7P3tAXimVroy7s.jpeg)
然后需要适量Fe3O4粉，325目及以下就可以了（50g 4元左右）
![](https://image.lceda.cn/pullimage/SvESL8rtMG1aqxsBP6iwBPlRBgWHc1pFuMlorjJ8.jpeg)
混在一起，搅拌直到均匀
最后，开始涂到墙上，记得要多涂几遍
![](https://image.lceda.cn/pullimage/ITCV1IyqjDAuJjjTdUJrLLZoZrROuwwjIQikpIXu.jpeg)
一定要用美纹纸确定区域，防止边界参差不齐。。。