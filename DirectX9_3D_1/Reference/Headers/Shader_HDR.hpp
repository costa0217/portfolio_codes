
//------------------------------------------------------------------
//  GLOBAL VARIABLES
//------------------------------------------------------------------
texture				g_HDRTexture;
sampler OriSampler = sampler_state
{
	texture = g_HDRTexture;

	minfilter = linear; magfilter = linear;	mipfilter = linear;
	AddressU = Clamp;	AddressV = Clamp;
};
texture				g_LumTexture;
sampler LumSampler = sampler_state
{
	texture = g_LumTexture;

	minfilter = linear;	magfilter = linear;	mipfilter = linear;
	AddressU = Clamp;	AddressV = Clamp;
};
texture				g_BloomTexture;
sampler BloomSampler = sampler_state
{
	texture = g_BloomTexture;

	minfilter = linear;	magfilter = linear;	mipfilter = linear;
	AddressU = Clamp;	AddressV = Clamp;
};

texture		g_DepthTexture;
texture		g_AlbedoTexture;
sampler AlbedoSampler = sampler_state
{
	texture = g_AlbedoTexture;
};
sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;
};


float       fExposure;                          // A user configurable bias to under/over expose the image
float       fGaussianScalar;                    // Used in the post-processing, but also useful here
float       g_rcp_bloom_tex_w;                  // The reciprocal WIDTH of the texture in 'bloom'
float       g_rcp_bloom_tex_h;                  // The reciprocal HEIGHT of the texture in 'bloom'

//------------------------------------------------------------------
//  SHADER ENTRY POINT
//------------------------------------------------------------------
float4 PS_Main( float2 UV : TEXCOORD0 ) : COLOR0
{
	UV.x += g_rcp_bloom_tex_w * 0.5f;
	UV.y += g_rcp_bloom_tex_h * 0.5f;
	float4 OriColor = tex2D( OriSampler, UV );
	float4 LumColor = tex2D( LumSampler, float2( 0.5f, 0.5f ) );

	// Compute the blur value using a bilinear filter
	// It is worth noting that if the hardware supports linear filtering of a
	// floating point render target that this step can probably be skipped.
	float xWeight	= frac( UV.x / g_rcp_bloom_tex_w ) - 0.5;
	float xDir		= xWeight;
	xWeight = abs( xWeight );
	xDir /= xWeight;
	xDir *= g_rcp_bloom_tex_w;

	float yWeight	= frac( UV.y / g_rcp_bloom_tex_h ) - 0.5;
	float yDir		= yWeight;
	yWeight	= abs( yWeight );
	yDir /= yWeight;
	yDir *= g_rcp_bloom_tex_h;

	// sample the blur texture for the 4 relevant pixels, weighted accordingly
	float4 b = ((1.0f - xWeight) * (1.0f - yWeight))    * tex2D( BloomSampler, UV );        
	b +=       (xWeight * (1.0f - yWeight))             * tex2D( BloomSampler, UV + float2( xDir, 0.0f ) );
	b +=       (yWeight * (1.0f - xWeight))             * tex2D( BloomSampler, UV + float2( 0.0f, yDir ) );
	b +=       (xWeight * yWeight)                      * tex2D( BloomSampler, UV + float2( xDir, yDir ) );

	// Compute the actual colour:
	float4 final = OriColor + 0.25f * b;

	float Lp = (fExposure / LumColor.r) * max( final.r, max( final.g, final.b ) );
	float LmSqr = (LumColor.g + fGaussianScalar * LumColor.g) * (LumColor.g + fGaussianScalar * LumColor.g);

	// Compute Eqn#3:
	float toneScalar = ( Lp * ( 4.0f + ( Lp / ( LmSqr ) ) ) ) / ( 1.0f + Lp );

	// Tonemap the final outputted pixel:
	OriColor = final * max(toneScalar, 1.4f);

	// Return the fully composed colour
	OriColor.a = 1.0f;


	// vignetting (중심으로 집중)
	float dist = distance(UV, float2(0.5,0.5)) * 0.8 ;    
	OriColor.rgb *= (1 - smoothstep(0, 1, dist));    

	return OriColor;
	//return vector(1.f, 0.f, 1.f, 1.f);

}

float4 PS_Effect( float2 UV : TEXCOORD0 ) : COLOR0
{

	float4 OriColor = tex2D( OriSampler, UV );

	// Compute the blur value using a bilinear filter
	// It is worth noting that if the hardware supports linear filtering of a
	// floating point render target that this step can probably be skipped.
	float xWeight	= frac( UV.x / g_rcp_bloom_tex_w ) - 0.5;
	float xDir		= xWeight;
	xWeight = abs( xWeight );
	xDir /= xWeight;
	xDir *= g_rcp_bloom_tex_w;

	float yWeight	= frac( UV.y / g_rcp_bloom_tex_h ) - 0.5;
	float yDir		= yWeight;
	yWeight	= abs( yWeight );
	yDir /= yWeight;
	yDir *= g_rcp_bloom_tex_h;

	// sample the blur texture for the 4 relevant pixels, weighted accordingly
	float4 b = ((1.0f - xWeight) * (1.0f - yWeight))    * tex2D( BloomSampler, UV );        
	b +=       (xWeight * (1.0f - yWeight))             * tex2D( BloomSampler, UV + float2( xDir, 0.0f ) );
	b +=       (yWeight * (1.0f - xWeight))             * tex2D( BloomSampler, UV + float2( 0.0f, yDir ) );
	b +=       (xWeight * yWeight)                      * tex2D( BloomSampler, UV + float2( xDir, yDir ) );

	// Compute the actual colour:
	float4 final = OriColor + 0.25f * b;

	// Tonemap the final outputted pixel:
	OriColor = final * 1.2f;

	// Return the fully composed colour
	OriColor.a = 1.0f;



	return OriColor;
}

technique Default
{
	// pass : 기능의 캡슐화
	pass FinalScene
	{		
		VertexShader = /*compile vs_3_0 VS_pass*/NULL;
		PixelShader = compile ps_3_0 PS_Main();
	}	
	pass FinalEffectScene
	{		
		VertexShader = /*compile vs_3_0 VS_pass*/NULL;
		PixelShader = compile ps_3_0 PS_Effect();
	}	
};
