#ifndef __AD2S1210_INS_H
#define __AD2S1210_INS_H

// 寄存器映射
// 只读
#define AD2S1210_PosHigh 				0x80   	// 位置高8位
#define AD2S1210_PosLow 				0x81	// 位置低8位
#define AD2S1210_VelHigh 				0x82	// 速度高8位
#define AD2S1210_VelLow 				0x83	// 速度低8位
#define AD2S1210_Fault					0xFF	// 故障

// 读写
#define AD2S1210_LOS 							0x88	// LOS阈值
#define AD2S1210_DOS_Over_Range_Threshold 		0x89	// DOS超量程阈值
#define AD2S1210_DOS_Mismatch_threshold			0x8A	// DOS失配阈值
#define AD2S1210_DOS_Reset_Maximum_Threshold	0x8B	// DOS复位最大阈值
#define AD2S1210_DOS_Reset_Minimum_Threshold	0x8C	// DOS复位最小阈值
#define AD2S1210_LOT_upper_Limit				0x8D	// LOT上限
#define AD2S1210_LOT_lower_Limit				0x8E	// LOT下限
#define AD2S1210_Excitation_Frequency			0x91	// 激励频率
#define AD2S1210_Control						0x92	// 控制

// 只写
#define AD2S1210_Soft_Reset						0xF0	// 软复位

#define AD2S1210_DUMMY_BYTE						0x00	

#endif
