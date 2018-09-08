#include "Shader_Setter.hpp"


//------------------------------------------------------------------
//  GLOBAL VARIABLES
//------------------------------------------------------------------
float fBrightPassThreshold;             // Values greater than this are accepted for the bright pass
float4 tcBrightOffsets[4];				// The sampling offsets used by 'BrightPass'
float4 tcDownSampleOffsets[16];         // The sampling offsets used by 'DownSample'

float HBloomWeights[9];                 // Description of the sampling distribution used by
float HBloomOffsets[9];                 // the HorizontalBlur() function

float VBloomWeights[9];                 // Description of the sampling distribution used by
float VBloomOffsets[9];                 // the VerticalBlur() function

static const int    MAX_SAMPLES = 16;    // 최대샘플링수
float2 g_avSampleOffsets[MAX_SAMPLES];	// 샘플링위치
float4 g_avSampleWeights[MAX_SAMPLES];	// 샘플링가중치

texture				g_BloomTexture;

sampler BloomSampler = sampler_state
{
	texture = g_BloomTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;

	AddressU = Clamp;
	AddressV = Clamp;
};


texture		g_CrossTexture1;
sampler s0 = sampler_state
{
	texture		= g_CrossTexture1;
	minfilter	= point;	magfilter = point;	mipfilter = point;
	AddressU = Clamp;	AddressV = Clamp;
};
texture		g_CrossTexture2;
sampler s1 = sampler_state
{
	texture		= g_CrossTexture2;
	minfilter	= point;	magfilter = point;	mipfilter = point;
	AddressU = Clamp;	AddressV = Clamp;
};
texture		g_CrossTexture3;
sampler s2 = sampler_state
{
	texture		= g_CrossTexture3;
	minfilter	= point;	magfilter = point;	mipfilter = point;
	AddressU = Clamp;	AddressV = Clamp;
};
texture		g_CrossTexture4;
sampler s3 = sampler_state
{
	texture		= g_CrossTexture4;
	minfilter	= point;	magfilter = point;	mipfilter = point;
	AddressU = Clamp;	AddressV = Clamp;
};
texture		g_CrossTexture5;
sampler s4 = sampler_state
{
	texture		= g_CrossTexture5;
	minfilter	= point;	magfilter = point;	mipfilter = point;
	AddressU = Clamp;	AddressV = Clamp;
};
texture		g_CrossTexture6;
sampler s5 = sampler_state
{
	texture		= g_CrossTexture6;
	minfilter	= point;	magfilter = point;	mipfilter = point;
	AddressU = Clamp;	AddressV = Clamp;
};

//------------------------------------------------------------------
// BRIGHT PASS AND 2x2 DOWN-SAMPLING PIXEL SHADER
//------------------------------------------------------------------
float4 PS_BrightPass(float2 UV : TEXCOORD0 ) : COLOR
{

	float4 average = { 0.0f, 0.0f, 0.0f, 0.0f };

	// load in and combine the 4 samples from the source HDR texture
	for( int i = 0; i < 4; i++ )
	{
		average += tex2D( BloomSampler, UV + float2( tcBrightOffsets[i].x, tcBrightOffsets[i].y ) );
	}
	average *= 0.25f;

	float luminance = max( average.r, max( average.g, average.b ) );

	// Determine whether this pixel passes the test...
	if( luminance < fBrightPassThreshold )
		average = float4( 0.0f, 0.0f, 0.0f, 1.0f );

	// Write the colour to the bright-pass render target
	return average;
}

//------------------------------------------------------------------
// 4x4 DOWN-SAMPLING PIXEL SHADER
//------------------------------------------------------------------
float4 PS_DownSample(float2 UV : TEXCOORD0 ) : COLOR
{

	float4 average = { 0.0f, 0.0f, 0.0f, 0.0f };

	for( int i = 0; i < 16; i++ )
	{
		average += tex2D( BloomSampler, UV + float2(tcDownSampleOffsets[i].x, tcDownSampleOffsets[i].y) );
	}

	average *= ( 1.0f / 16.0f );

	return average;
}
//------------------------------------------------------------------
// HORIZONTAL BLUR
//------------------------------------------------------------------
float4 PS_HorizontalBlur( float2 UV : TEXCOORD0 ) : COLOR
{
	float4 color = { 0.0f, 0.0f, 0.0f, 0.0f };

	for( int i = 0; i < 9; i++ )
	{
		color += (tex2D( BloomSampler, UV + float2( HBloomOffsets[i], 0.0f ) ) * HBloomWeights[i] );
	}
	return float4( color.rgb, 1.0f );
}
//------------------------------------------------------------------
// VERTICAL BLUR
//------------------------------------------------------------------
float4 PS_VerticalBlur( float2 UV : TEXCOORD0 ) : COLOR
{

	float4 color = { 0.0f, 0.0f, 0.0f, 0.0f };

	for( int i = 0; i < 9; i++ )
	{
		color += (tex2D( BloomSampler, UV + float2( 0.0f, VBloomOffsets[i] ) ) * VBloomWeights[i] );
	}

	return float4( color.rgb, 1.0f );

}

//-----------------------------------------------------------------------------
// Name: Star
// Desc: 8개를 샘플링해서 광선을 만든다
//-----------------------------------------------------------------------------
float4 PS_Star (float2 UV : TEXCOORD0 ) : COLOR
{
	float4 vColor = 0.0f;

	// 광선에 8개의 점을 샘플링
	for(int i = 0; i < 8; i++) 
	{
		vColor += g_avSampleWeights[i] * tex2D(BaseSampler, UV + g_avSampleOffsets[i]);
	}
	//vColor = tex2D(BaseSampler, UV);
	return vColor;
}
//-----------------------------------------------------------------------------
// Name: MergeTextures_6
// Desc: 6장의 광선을 중복합성
//-----------------------------------------------------------------------------
float4 PS_MergeTextures_6(float2 UV : TEXCOORD0 ) : COLOR
{
	float4 vColor = 0.0f;

	vColor = ( tex2D(s0, UV)
		+ tex2D(s1, UV)
		+ tex2D(s2, UV)
		+ tex2D(s3, UV)
		+ tex2D(s4, UV)
		+ tex2D(s5, UV) )/6.0f;

	return vColor;
}



technique Default
{
	// pass : 기능의 캡슐화
	
	pass BrightPass
	{		
		VertexShader = /*compile vs_3_0 VS_pass*/NULL;
		PixelShader = compile ps_3_0 PS_BrightPass();
	}	
	pass DownSamplePass
	{		
		VertexShader = /*compile vs_3_0 VS_pass*/NULL;
		PixelShader = compile ps_3_0 PS_DownSample();
	}	
	pass HBlurPass
	{		
		VertexShader = /*compile vs_3_0 VS_pass*/NULL;
		PixelShader = compile ps_3_0 PS_HorizontalBlur();
	}
	pass VBlurPass
	{		
		VertexShader = /*compile vs_3_0 VS_pass*/NULL;
		PixelShader = compile ps_3_0 PS_VerticalBlur();
	}

	pass CrossFillterPass
	{
		VertexShader = /*compile vs_3_0 VS_pass*/NULL;
		PixelShader  = compile ps_3_0 PS_Star();
	}

	pass MergeFillterPass
	{
		VertexShader = /*compile vs_3_0 VS_pass*/NULL;
		PixelShader  = compile ps_3_0 PS_MergeTextures_6();
	}
};
