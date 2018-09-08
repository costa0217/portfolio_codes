// 전역변수 (Constant Table)
matrix			g_matWorld;
matrix			g_matView;
matrix			g_matProj;

int					g_ICntX;
int					g_ICntY;

int					g_iFrameCnt;
int					g_iFrameHeight;
float				g_fAlpha;
vector			g_vColor;

texture			g_BaseTexture;
sampler BaseSampler = sampler_state{
	texture = g_BaseTexture;

	minfilter		= linear;
	magfilter		= linear;
	mipfilter		= linear;
};

texture			g_DepthTexture;
sampler DepthSampler = sampler_state{
	texture = g_DepthTexture;
};

vector		g_vCamDir;

// VertexShading의 입력값으로 사용될 정점의 정보
struct VS_IN
{
	vector		vPosition : POSITION;
	vector		vNormal	: NORMAL;
	float2			vUV : TEXCOORD0;
};

struct VS_OUT
{
	vector		vPosition : POSITION;
	float2			vUV : TEXCOORD0;
	vector		vClipPos	: TEXCOORD1;
	float2			vRealUV	: TEXCOORD2;
	vector		vWPos	: TEXCOORD3;
};


// pass1 Alpha ////////////////////////////////////////////////////
VS_OUT VS_Alpha(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	// matWV : 월드 * 뷰
	// matWVP : 월드 * 뷰 * 투영
	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);	

	Out.vPosition	= mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vClipPos	= mul(vector(In.vPosition.xyz, 1.f), matWV);

	Out.vUV.x = (In.vUV.x + g_iFrameCnt) / g_ICntX;
	Out.vUV.y = (In.vUV.y + g_iFrameHeight) / g_ICntY;

	Out.vWPos = Out.vPosition;

	Out.vRealUV = In.vUV;

	return Out;
}
struct PS_IN 
{
	float2		vUV : TEXCOORD0;
	vector	vClipPos : TEXCOORD1;
	float2		vRealUV : TEXCOORD2;
	vector	vWPos : TEXCOORD3;
};

struct PS_OUT
{
	vector		vColor : COLOR0;
	vector		vDepth : COLOR1;
};


PS_OUT PS_Alpha(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	
	float2 uv		= In.vWPos.xy / In.vWPos.w;
	uv.y			= -uv.y;
	uv				= uv * 0.5f + 0.5f;
	vector	vObjDepth = tex2D(DepthSampler, uv);

	//float	d = distance(vObjDepth.a, In.vWPos.z * 0.001f);

	Out.vColor = tex2D(BaseSampler, In.vUV);
	Out.vColor *= g_vColor;
	//Out.vColor.a	= Out.vColor.r + Out.vColor.g + Out.vColor.b;

	clip(Out.vColor.a < 0.2f  ? -1 : 1);

	Out.vDepth	=  vector(In.vClipPos.z * 0.001f, In.vClipPos.z * 0.001f, 1.f, 1.f)/*vGBufferDepth*/;

	float D = 1.f;	
	D = saturate((vObjDepth.r - Out.vDepth.r) * 800.f);
	if(Out.vColor.a > D)
		Out.vColor.a = D;

	return Out;
}

// pass2  black ////////////////////////////////////////////////////
VS_OUT VS_Black(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	// matWV : 월드 * 뷰
	// matWVP : 월드 * 뷰 * 투영
	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);	

	Out.vPosition	= mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vClipPos	= mul(vector(In.vPosition.xyz, 1.f), matWV);

	Out.vUV.x = (In.vUV.x + g_iFrameCnt) / g_ICntX;
	Out.vUV.y = (In.vUV.y + g_iFrameHeight) / g_ICntY;

	Out.vRealUV = In.vUV;
	Out.vWPos = Out.vPosition;

	return Out;
}
PS_OUT PS_Black(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float2 uv		= In.vWPos.xy / In.vWPos.w;
	uv.y			= -uv.y;
	uv				= uv * 0.5f + 0.5f;
	vector	vObjDepth = tex2D(DepthSampler, uv);
	Out.vColor = tex2D(BaseSampler, In.vUV);

	Out.vColor *= g_vColor;
	Out.vColor.a	= Out.vColor.r + Out.vColor.g + Out.vColor.b;

	clip(Out.vColor.a < 0.2f  ? -1 : 1);

	//float	d = distance(vObjDepth.a, In.vWPos.z * 0.001f);
	Out.vDepth		=  vector(In.vClipPos.z * 0.001f, In.vClipPos.z * 0.001f, 1.f, 1.f)/*vGBufferDepth*/;


	float D = 1.f;	
	D = saturate((vObjDepth.r - Out.vDepth.r) * 800.f);
	if(Out.vColor.a > D)
		Out.vColor.a = D;

	return Out;
}
// pass 3 /////////////////////////////////

struct VS_No_OUT
{
	vector		vPosition			: POSITION;
	float2			vUV				: TEXCOORD0;
	vector		vWorldNormal	: TEXCOORD1;
	float2			vRealUV			: TEXCOORD2;
	vector		vClipPos			: TEXCOORD3;
	vector		vWPos			: TEXCOORD4;
};
VS_No_OUT VS_NoHDR(VS_IN In)
{
	VS_No_OUT		Out = (VS_No_OUT)0;

	matrix		matWV, matWVP;

	matWV	= mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);	

	Out.vPosition			= mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vWorldNormal		= /*mul(g_vCamDir, g_matWorld)*/ normalize(g_vCamDir - mul(vector(In.vPosition.xyz, 1.f), g_matWorld));
	Out.vClipPos			= mul(vector(In.vPosition.xyz, 1.f), matWV);

	Out.vUV.x = (In.vUV.x + g_iFrameCnt) / g_ICntX;
	Out.vUV.y = (In.vUV.y + g_iFrameHeight) / g_ICntY;

	Out.vRealUV	= In.vUV;
	Out.vWPos		= Out.vPosition;
	return Out;
}

struct PS_No_IN 
{
	float2		vUV				: TEXCOORD0;
	vector	vWorldNormal	: TEXCOORD1;
	float2		vRealUV			: TEXCOORD2;
	vector	vClipPos			: TEXCOORD3;
	vector	vWPos			: TEXCOORD4;
};

struct PS_No_OUT
{
	vector		vColor	: COLOR0;
	vector		vNormal	: COLOR1;
	vector		vDepth	: COLOR2;
	//vector		vFirstEffect	: COLOR3;
};

PS_No_OUT PS_NoHDR(PS_No_IN In)
{
	PS_No_OUT		Out = (PS_No_OUT)0;

	Out.vColor		= tex2D(BaseSampler, In.vUV);

	Out.vColor.rgb -= float3(0.4f, 0.4f,0.4f);

	float2 uv		= In.vWPos.xy / In.vWPos.w;
	uv.y			= -uv.y;
	uv				= uv * 0.5f + 0.5f;
	vector	vObjDepth = tex2D(DepthSampler, uv);

	Out.vColor = tex2D(BaseSampler, In.vUV);
	Out.vColor *= g_vColor;
	Out.vDepth	=  vector(In.vClipPos.z * 0.001f, In.vClipPos.z * 0.001f, 1.f, 1.f)/*vGBufferDepth*/;

	float D = 1.f;	
	D = saturate((vObjDepth.r - Out.vDepth.r) * 800.f);
	if(Out.vColor.a > D)
		Out.vColor.a = D;

	//Out.vDepth	= vector(0.f, 0.f, 0.f, Out.vColor.a);

	return Out;
}
PS_No_OUT PS_NoHDRClip(PS_No_IN In)
{
	PS_No_OUT		Out = (PS_No_OUT)0;

	Out.vColor		= tex2D(BaseSampler, In.vUV);
	vector	vObjDepth = tex2D(DepthSampler, In.vRealUV);
	/*float D = 1.f;	
	D = saturate((vObjDepth.r - In.vClipPos.z * 0.001f) * 200.f);
	if(Out.vColor.a > D)
		Out.vColor.a = D;*/

	clip(Out.vColor.a < 0.24f ? -1 : 1);
	Out.vNormal	= vector(In.vWorldNormal.xyz, saturate(Out.vColor.a - 0.2f));
	Out.vDepth		= vector(In.vClipPos.z * 0.001f, In.vClipPos.z * 0.001f, 1.f, 1.f);
	//Out.vDepth		= vector(0.f, 0.f, 0.f, Out.vColor.a);
	return Out;
}



// technique : 장치의 지원여부에따라 다른 셰이딩을 할 수 있도록 한다.
technique Default
{
	// pass : 기능의 캡슐화
	pass Render_RcAlpha
	{		
		CullMode = none;

		AlphaBlendenable = true;
		Srcblend = srcalpha;
		Destblend = invsrcalpha;

		VertexShader	= compile vs_3_0 VS_Alpha();
		PixelShader		= compile ps_3_0 PS_Alpha();
	}
	pass Render_RcBlack
	{		
		CullMode = none;

		AlphaBlendenable = true;
		Srcblend = srcalpha;
		Destblend = invsrcalpha;

		VertexShader	= compile vs_3_0 VS_Black();
		PixelShader		= compile ps_3_0 PS_Black();
	}
	pass Render_RcNoHDR
	{		
		CullMode = none;

		AlphaBlendenable = true;
		Srcblend = srcalpha;
		Destblend = invsrcalpha;

		VertexShader	= compile vs_3_0 VS_NoHDR();
		PixelShader		= compile ps_3_0 PS_NoHDR();
	}
	pass Render_RcNoHDRClip
	{		
		CullMode = none;

		AlphaBlendenable = true;
		Srcblend = srcalpha;
		Destblend = invsrcalpha;

		VertexShader	= compile vs_3_0 VS_NoHDR();
		PixelShader		= compile ps_3_0 PS_NoHDRClip();
	}
};
