这是一个网络天气站程序
时间：2022/10/11
创建者：Yfc

LCD字符取模格式
点阵格式：阴码（点亮为1）
取模方式：逐行式
取模走向：逆向（低位在前）

接收数据格式
//1、心知天气

 (1) 获取当前天气
eg: https://api.seniverse.com/v3/weather/now.json?key=SEiJPSGoiKojJEp-x&location=guangyuan&language=en&unit=c
{"results":
	[
		{
			"location":
			{
				"id":"WTMYGTZ7WZMY",
				"name":"Jiaxing",
				"country":"CN",
				"path":"Jiaxing,Jiaxing,Zhejiang,China",
				"timezone":"Asia/Shanghai",
				"timezone_offset":"+08:00"
			},
			"now":
			{
				"text":"Clear",
				"code":"1",
				"temperature":"12"
			},
			"last_update":
				"2023-01-11T18:42:35+08:00"
		}
	]
}

 (2) 获取预报天气
eg: https://api.seniverse.com/v3/weather/daily.json?key=SEiJPSGoiKojJEp-x&location=guangyuan&language=en&unit=c&start=0&days=3

{
    "results":
    [
        {
            "location":
            {
                "id":"WMG22PXN3WPT",
                "name":"Guangyuan",
                "country":"CN",
                "path":"Guangyuan,Guangyuan,Sichuan,China",
                "timezone":"Asia/Shanghai",
                "timezone_offset":"+08:00"
            },
            "daily":
            [
                {
                    "date":"2023-01-17",    //第11个“:”
                    "text_day":"Cloudy",
                    "code_day":"4",
                    "text_night":"Overcast",
                    "code_night":"9",
                    "high":"8",             //第16个“:”
                    "low":"-3",             //第17个“:”
                    "rainfall":"0.00",
                    "precip":"0.00",
                    "wind_direction":"CLM",
                    "wind_direction_degree":"",
                    "wind_speed":"8.4",
                    "wind_scale":"2",
                    "humidity":"42"         //第24个“:”
                },
                {
                    "date":"2023-01-18",
                    "text_day":"Sunny",
                    "code_day":"0",
                    "text_night":"Cloudy",
                    "code_night":"4",
                    "high":"8",
                    "low":"-1",
                    "rainfall":"0.00",
                    "precip":"0.00",
                    "wind_direction":"CLM",
                    "wind_direction_degree":"",
                    "wind_speed":"8.4",
                    "wind_scale":"2",
                    "humidity":"50"
                },
                {
                    "date":"2023-01-19",
                    "text_day":"Sunny",
                    "code_day":"0",
                    "text_night":"Overcast",
                    "code_night":"9",
                    "high":"10",
                    "low":"0",
                    "rainfall":"0.00",
                    "precip":"0.00",
                    "wind_direction":"CLM",
                    "wind_direction_degree":"",
                    "wind_speed":"3.0",
                    "wind_scale":"1",
                    "humidity":"58"
                }
            ],
            "last_update":"2023-01-17T08:00:00+08:00"
        }
    ]
}

版本修订：
NWS_stm32：
            v1.0------适用128x128彩色屏幕
            v2.0------适用240x240彩色屏幕
            v2.1------适用240x240彩色屏幕，添加esp8366 wifi 模块 自动获取时间
            v2.2------适用240x240彩色屏幕，添加esp8366 wifi 模块 自动获取天气
            v2.3------适用240x240彩色屏幕，完善了天气显示部分与wifi连接显示代码
            v2.4------适用240x240彩色屏幕，完善了最高温最低温及湿度数据获取与显示
            v2.5------适用240x240彩色屏幕，添加了W25Q128存储图片数据
            v2.6------适用240x240彩色屏幕，将屏幕背景更改为白色
            v2.7------在v2.6基础上去除多余库函数
			v2.8------添加外部 flash 字库
			v2.9------添加外部 flash FatFs文件系统以及相关二进制图片文件
			v3.0------完善了整个程序架构
Picture_Upload_stm32:
            v1.0------图片上串至 wq25x flash存储芯片	
			
外部FLASH存储格式：

数据功能		起始地址		长度
测试区	0		4096
ASCLL_H16		4096			3*1024 (3KB)
ASCLL_H32		8*1024			9*1024 (9KB)
ASCLL_H48		20*1024			19*1024 (19KB)
GB2312_H16		40*1024			256*1024 (256KB)
GB2312_H24		300*1024		576*1024 (576KB)
GB2312_H32		80*1024			1023*1024 (1023KB)
		
注意：扇区擦除时空间分配不合理可能重复擦除，要考虑扇区对齐		
