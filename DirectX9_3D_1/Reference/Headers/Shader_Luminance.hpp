#include "Shader_Setter.hpp"

// The offsets used by GreyScaleDownSample()
float4      tcLumOffsets[4];
// The offsets used by DownSample()
float4      tcDSOffsets[9];                 


float4 PS_GreyScaleDownSample(float2 UV : TEXCOORD0) : COLOR0
{
	//// Compute the average of the 4 necessary samples
	float average = 0.0f;
	float maximum = -1e20;
	float4 color = 0.0f;
	float3 WEIGHT = float3( 0.299f, 0.587f, 0.114f) ; // ÈÖµµ»ó¼ö

	for( int i = 0; i < 4; ++i )
	{
		color = tex2D( BaseSampler, UV + float2( tcLumOffsets[i].x, tcLumOffsets[i].y ) );

		// 3. Take the maximum value of the incoming, same as computing the
		//    brightness/value for an HSV/HSB conversion:
		float GreyValue = max( color.r, max( color.g, color.b ) );

		//float GreyValue = dot( color.rgb, WEIGHT );

		maximum = max( maximum, GreyValue );
		average += (0.25f * log( 1e-5 + GreyValue )); //1e-5 necessary to stop the singularity at GreyValue=0
	}
	average = exp( average );

	
	
	return vector(average, maximum, 0.f, 1.f);
	//return float4( 0.22f, 0.22f, 0.22f, 1.0f );
}

float4 PS_DownSample( float2 UV : TEXCOORD0 ) : COLOR0
{

	// Compute the average of the 10 necessary samples
	float4 color = 0.0f;
	float maximum = -1e20;
	float average = 0.0f;

	for( int i = 0; i < 9; i++ )
	{
		color = tex2D( BaseSampler, UV + float2( tcDSOffsets[i].x, tcDSOffsets[i].y ) );
		average += color.r;
		maximum = max( maximum, color.g );
	}
	average /= 9.0f;
	

	// Return the new average luminance
	return vector(average, maximum, 0.f, 1.f);
}

technique Default
{
	// pass : ±â´ÉÀÇ Ä¸½¶È­
	
	pass GreyScaleDownSample
	{		
		VertexShader = /*compile vs_3_0 VS_pass*/NULL;
		PixelShader = compile ps_3_0 PS_GreyScaleDownSample();
	}	
	pass DownSample
	{		
		VertexShader = /*compile vs_3_0 VS_pass*/NULL;
		PixelShader = compile ps_3_0 PS_DownSample();
	}
};
