// 전역변수 (Constant Table)
matrix			g_matWorld;
matrix			g_matView;
matrix			g_matProj;

texture			g_BaseTexture;

sampler BaseSampler = sampler_state
{
	texture = g_BaseTexture;
};

vector				g_vLightDir; // 방향성광원의 방향벡터를 얻어오겠다.
vector				g_vLightDiffuse; // 광원의 난반사 색상을 얻어오자.
vector				g_vLightAmbient;
vector				g_vLightSpecular;

vector				g_vMeshDiffuse; // 지형객체의 난반사 색상을 얻어온다.
vector				g_vMeshAmbient;
vector				g_vMeshSpecular;
vector				g_vCameraPos; // 카메라 포즈 데스

// VertexShading의 입력값으로 사용될 정점의 정보
struct VS_IN
{
	vector		vPosition : POSITION;
	vector		vNormal	: NORMAL;
	float2		vUV : TEXCOORD0;
};

struct VS_OUT
{
	vector		vPosition : POSITION;
	float2		vUV : TEXCOORD0;
};


// VertexShading : 정점의 변환 (영역의 변환)
VS_OUT/*정점*/ VS_MAIN(VS_IN In/*정점*/)
{
	VS_OUT		Out = (VS_OUT)0;

	// matWV : 월드 * 뷰
	// matWVP : 월드 * 뷰 * 투영
	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);

	matWVP = mul(matWV, g_matProj);	

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);

	Out.vUV = In.vUV;

	return Out;
}

// COLOR, TEXCOORD
struct PS_IN 
{
	float2		vUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : COLOR0;
};


PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vUV);

	Out.vColor	   -= g_vLightDiffuse;
	//Out.vColor.a   = 0.f;

	return Out;
}

// technique : 장치의 지원여부에따라 다른 셰이딩을 할 수 있도록 한다.
technique Default
{
	// pass : 기능의 캡슐화
	pass Render_RcTex
	{		
		Alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
};