//=============================================================================
// Basic.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// �ϴ� �� �������� ���̴� ȿ��
//=============================================================================

#include "LightHelper.fx"
 
cbuffer cbPerFrame
{
    float4x4 gWorldViewProj;
};

TextureCube gCubeMap;
int         gLightCount;

SamplerState samTriLinearSam
{
    Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

struct VertexIn
{
	float3 PosL    : POSITION;
};

struct VertexOut
{
	float4 PosH    : SV_POSITION;
    float3 PosL    : POSITION;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// z/w = �� �ǵ���(�� �ϴ� ���� �׻� �� ��鿡 �ֵ���) z = w �� �����Ѵ�.
    vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj ).xyww;
    vout.PosL = vin.PosL;
    
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    return gCubeMap.Sample(samTriLinearSam, pin.PosL);
}

RasterizerState NoCull
{
    CullMode = None;
};

DepthStencilState LessEqualDSS
{
    // ���� �Լ��� �׳� LESS_EQUAL�� �ؾ� �Ѵ�.
    // �׷��� ���� ������, ���� ���۸� 1�� �����ٰ� �� �� z = 1 (NDC)��
    // ����ȭ�� ���� ������ ���� ������ �����Ѵ�.
    DepthFunc = LESS_EQUAL;
};

technique11 SkyTech
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS()));
        
        SetRasterizerState(NoCull);
        SetDepthStencilState(LessEqualDSS, 0);
    }
}