#ifndef MESH_DEFINE_H
#define MESH_DEFINE_H

/****上报数据类型码*****/
#define CMD_MARK_CODE_LIGHT_STATUS                  ((char)0xDC)  //灯状态上报
#define CMD_MARK_CODE_USER_NOTIFY_DATA              ((char)0xEA)  //用户数据上报
#define CMD_MARK_CODE_GROUP_G8_NOTIFY_DATA          ((char)0xD4)  //组状态上报

/****用户数据类型码****/
#define USER_NOTIFY_DATA_OF_LIGHT                   ((char)0x01)  //灯光参数
#define USER_NOTIFY_DATA_OF_MOTION_SENSOR           ((char)0x02)  //微波人体移动传感器
#define USER_NOTIFY_DATA_OF_WARNING_LIGHT           ((char)0x02)  //报警灯
#define USER_NOTIFY_DATA_OF_LUX_SENSOR              ((char)0x03)  //光照传感器
#define USER_NOTIFY_DATA_OF_CO1_SENSOR              ((char)0x04)  //CO1传感器
#define USER_NOTIFY_DATA_OF_CO2_SENSOR              ((char)0x05)  //CO2传感器
#define USER_NOTIFY_DATA_OF_PM_SENSROR              ((char)0x06)  //PM传感器
#define USER_NOTIFY_DATA_OF_SWITCH_SENSOR           ((char)0x09)  //开关传感器
#define USER_NOTIFY_DATA_OF_SWITCH_SENSOR_2CH               ((char)0x11)  //2路开关传感器
#define USER_NOTIFY_DATA_OF_FLAMMABLE_GAS           ((char)0x12)  //Flammable Gas Sensor
#define USER_NOTIFY_DATA_OF_MOTION_AND_LUX_CAMERA_SENSOR    ((char)0x07) //人体感应和光照摄像头
#define USER_NOTIFY_DATA_OF_TEMPERATURE_HUMIDITY_SENSOR     ((char)0x08) //温湿度传感器
#define USER_NOTIFY_DATA_OF_STRESS_SENSOR           ((char)0x0B) //应力传感器
#define USER_NOTIFY_DATA_OF_4CH_RELAY               ((char)0x0D) //四通道继电器
#define USER_NOTIFY_DATA_OF_SOLAR_1CH_LUMINAIRE     ((char)0x0E) //一通道太阳能路灯
#define USER_NOTIFY_DATA_OF_FORMALDEHYDE_SENSOR     ((char)0x0F)  //甲醛传感器
#define USER_NOTIFY_DATA_OF_ADBOARD                 ((char)0x10) //广告灯箱
#define USER_NOTIFY_DATA_OF_UVLIGHT_SENSOR          ((char)0x21) //Ultraviolet Light Sensor
#define USER_NOTIFY_DATA_OF_HAND_WASHING_SENSOR     ((char)0x22) //Hand Washing Sensor
#define USER_NOTIFY_DATA_OF_REFRG_TEMPERATURE_HUMIDITY_SENSOR     ((char)0x23) //Refrigerator Temperature Humidity Sensor
#define USER_NOTIFY_DATA_OF_UV_STERILIZER           ((char)0x24) //紫外刀具消毒柜
#define USER_NOTIFY_DATA_OF_OZONE                   ((char)0x0A) //Ozone Sensor

/****网关类型码*****/
#define NODE_TYPE_OF_USB_GATEWAY            ((char)0xA0) //网关

/****执行器类型码******/
#define NODE_TYPE_OF_LIGHT                  ((char)0xFF) //一通道灯
#define NODE_TYPE_OF_WARNING_LIGHT          ((char)0xF7)  //报警灯
#define NODE_TYPE_OF_4CH_RELAY              ((char)0xFE) //四通道继电器
#define NODE_TYPE_OF_SOLAR_1CH_LUMINAIRE    ((char)0xFD)   //一通道太阳能路灯
#define NODE_TYPE_OF_1CH_RELAY              ((char)0xFC) //一通道继电器
#define NODE_TYPE_OF_ADBOARD                ((char)0xFA) //广告灯箱
#define NODE_TYPE_OF_UV_STERILIZER          ((char)0xF3) //紫外刀具消毒柜
#define NODE_TYPE_OF_DRY_CONTACT_RELAY      ((char)0xF5) //Dry Contact Relay

/****传感器类型码*******/
#define NODE_TYPE_OF_MOTION_SENSOR           ((char)0x9E)  //微波人体移动传感器
#define NODE_TYPE_OF_LUX_SENSOR              ((char)0x9F)  //光照传感器
#define NODE_TYPE_OF_CO1_SENSOR              ((char)0x9D)  //CO1传感器
#define NODE_TYPE_OF_CO2_SENSOR              ((char)0x9C)  //CO2传感器
#define NODE_TYPE_OF_PM_SENSROR              ((char)0x9B)  //PM传感器
#define NODE_TYPE_OF_SWITCH_SENSOR           ((char)0x7F)  //开关传感器
#define NODE_TYPE_OF_SWITCH_SENSOR_2CH               ((char)0x7E)  //2路开关传感器
#define NODE_TYPE_OF_MOTION_AND_LUX_CAMERA_SENSOR    ((char)0x99) //人体感应和光照摄像头
#define NODE_TYPE_OF_TEMPERATURE_HUMIDITY_SENSOR     ((char)0x9A) //温湿度传感器
#define NODE_TYPE_OF_STRESS_SENSOR           ((char)0x98) //应力传感器
#define NODE_TYPE_OF_FORMALDEHYDE_SENSOR        ((char)0x97)  //甲醛传感器
#define NODE_TYPE_OF_REFRG_TEMPERATURE_HUMIDITY_SENSOR        ((char)0x93)    //Refrigerator Temperature Humidity Sensor
#define NODE_TYPE_OF_ULTRAVIOLET_LIGHT_SENSOR   ((char)0x94)    //Ultraviolet Light Sensor
#define NODE_TYPE_OF_HAND_WASHING_SENSOR        ((char)0x95)    //Hand Washing Sensor
#define NODE_TYPE_OF_OZONE_SENSOR               ((char)0x92)    //OZONE Sensor
#define NODE_TYPE_OF_FLAMMABLE_GAS_SENSOR       ((char)0x91)    //Flammable Gas Sensor

#endif // MESH_DEFINE_H
