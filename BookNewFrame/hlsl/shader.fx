
cbuffer ConstanceBuffer : register( b0 )
{
	matrix world;
	matrix view;
	matrix projection;
}

struct VS_INPUT
{
    float4 pos:POSITION;
	float3 normal:NORMAL;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 normal:TEXCOORD0;
	float3 color:COLOR;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT i)
{
    PS_INPUT o;
    o.Pos=mul(i.pos,world);
	o.color = float3(o.Pos.x, o.Pos.y, o.Pos.z);
	o.normal = mul(i.normal,world);
    o.Pos=mul(o.Pos,view);
    o.Pos=mul(o.Pos,projection);
    return o;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

float4 PS(PS_INPUT input) : SV_Target
{
    return float4(input.color,1.0f);
}


//--------------------------------------------------------------------------------------
// Technique Pass
//--------------------------------------------------------------------------------------

technique11 ColorTech
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
};