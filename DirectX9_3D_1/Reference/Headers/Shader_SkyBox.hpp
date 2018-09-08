// �������� (Constant Table)
matrix			g_matWorld;
matrix			g_matView;
matrix			g_matProj;

texture			g_BaseTexture;


vector			g_LightDir;

sampler BaseSampler = sampler_state
{
	texture = g_BaseTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

// VertexShading�� �Է°����� ���� ������ ����
struct VS_IN
{
	vector		vPosition : POSITION;
	vector		vNormal	: NORMAL;
	float3			vUV : TEXCOORD0;
};

struct VS_OUT
{
	vector		vPosition		: POSITION;
	float3			vUV			: TEXCOORD0;
	vector		vViewPos	: TEXCOORD1;
	float3			vNormal		: TEXCOORD2;
};


// VertexShading : ������ ��ȯ (������ ��ȯ)
VS_OUT/*����*/ VS_MAIN(VS_IN In/*����*/)
{
	VS_OUT		Out = (VS_OUT)0;

	// matWV : ���� * ��
	// matWVP : ���� * �� * ����
	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);	

	Out.vViewPos	= mul(In.vPosition, matWV);
	Out.vPosition	= mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vNormal	= g_LightDir.xyz;

	Out.vUV = In.vUV;

	return Out;
}

// COLOR, TEXCOORD
struct PS_IN 
{
	float3		vUV			: TEXCOORD0;
	vector	vViewPos	: TEXCOORD1;
	float3		vNormal		: TEXCOORD2;
};

struct PS_OUT
{
	vector		vColor	: COLOR0;
	vector		vNormal	: COLOR1;
	vector		vDepth	: COLOR2;
	//vector		vEffect	: COLOR3;
};


PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = texCUBE(BaseSampler, In.vUV);

	Out.vColor.a	= 0.f;
	/*Out.vNormal	= 0.f;*/
	Out.vDepth		= 1.f;

	return Out;
}

// technique : ��ġ�� �������ο����� �ٸ� ���̵��� �� �� �ֵ��� �Ѵ�.
technique Default
{
	// pass : ����� ĸ��ȭ
	pass Skybox
	{	
		CullMode = none;

		Zenable = false;
		Zwriteenable = false;

		VertexShader	= compile vs_3_0 VS_MAIN();
		PixelShader		= compile ps_3_0 PS_MAIN();
	}
};