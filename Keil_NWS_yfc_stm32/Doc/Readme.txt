����һ����������վ����
ʱ�䣺2022/10/11
�����ߣ�Yfc

LCD�ַ�ȡģ��ʽ
�����ʽ�����루����Ϊ1��
ȡģ��ʽ������ʽ
ȡģ�������򣨵�λ��ǰ��

�������ݸ�ʽ
//1����֪����

 (1) ��ȡ��ǰ����
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

 (2) ��ȡԤ������
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
                    "date":"2023-01-17",    //��11����:��
                    "text_day":"Cloudy",
                    "code_day":"4",
                    "text_night":"Overcast",
                    "code_night":"9",
                    "high":"8",             //��16����:��
                    "low":"-3",             //��17����:��
                    "rainfall":"0.00",
                    "precip":"0.00",
                    "wind_direction":"CLM",
                    "wind_direction_degree":"",
                    "wind_speed":"8.4",
                    "wind_scale":"2",
                    "humidity":"42"         //��24����:��
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

�汾�޶���
NWS_stm32��
            v1.0------����128x128��ɫ��Ļ
            v2.0------����240x240��ɫ��Ļ
            v2.1------����240x240��ɫ��Ļ�����esp8366 wifi ģ�� �Զ���ȡʱ��
            v2.2------����240x240��ɫ��Ļ�����esp8366 wifi ģ�� �Զ���ȡ����
            v2.3------����240x240��ɫ��Ļ��������������ʾ������wifi������ʾ����
            v2.4------����240x240��ɫ��Ļ�����������������¼�ʪ�����ݻ�ȡ����ʾ
            v2.5------����240x240��ɫ��Ļ�������W25Q128�洢ͼƬ����
            v2.6------����240x240��ɫ��Ļ������Ļ��������Ϊ��ɫ
            v2.7------��v2.6������ȥ������⺯��
			v2.8------����ⲿ flash �ֿ�
			v2.9------����ⲿ flash FatFs�ļ�ϵͳ�Լ���ض�����ͼƬ�ļ�
			v3.0------��������������ܹ�
Picture_Upload_stm32:
            v1.0------ͼƬ�ϴ��� wq25x flash�洢оƬ	
			
�ⲿFLASH�洢��ʽ��

���ݹ���		��ʼ��ַ		����
������	0		4096
ASCLL_H16		4096			3*1024 (3KB)
ASCLL_H32		8*1024			9*1024 (9KB)
ASCLL_H48		20*1024			19*1024 (19KB)
GB2312_H16		40*1024			256*1024 (256KB)
GB2312_H24		300*1024		576*1024 (576KB)
GB2312_H32		80*1024			1023*1024 (1023KB)
		
ע�⣺��������ʱ�ռ���䲻��������ظ�������Ҫ������������		
