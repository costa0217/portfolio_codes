matrix				g_matWorld;
matrix				g_matView;
matrix				g_matProj;

texture				g_BaseTexture;

sampler BaseSampler = sampler_state
{
	texture		= g_BaseTexture;
	minfilter		= linear;
	magfilter		= linear;
	mipfilter		= linear;
};

//float	fFar;		// 원평면
//float	fNear;		// 근평면
//
//#define PROJECTION_A  fFar / (fFar - fNear)
//#define PROJECTION_B  (-fNear) / (fFar - fNear)
//
//float GetLineardepth(float InDepth){return PROJECTION_B / (InDepth - PROJECTION_A);}
//float GetInvLineardepth(float InDepth){return (PROJECTION_B / InDepth) + PROJECTION_A;}
