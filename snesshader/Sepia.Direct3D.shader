shader~ language=HLSL
	texture rubyTexture;
	
	float4 vec;
	
	sampler s0 = sampler_state { texture = <rubyTexture>; };
	float3 LightColor = { 1.0, 0.7, 0.5 };
	float3 DarkColor = { 0.2, 0.05, 0.0 };
	
	float4 DiffColorPass(in float2 Tex : TEXCOORD0) : COLOR0
	{
		vec.x = 0.5;
		vec.y = 1.0;
		float3 scnColor = LightColor * tex2D(s0, Tex).xyz;
		float3 grayXfer = float3(0.3, 0.59, 0.11);
		float gray = dot(grayXfer, scnColor);
		float3 muted = lerp(scnColor, gray.xxx, vec.x);
		float3 sepia = lerp(DarkColor, LightColor, gray);
		float3 result = lerp(muted, sepia, vec.y);
		return float4(result, 1);
	}
	
	Technique T0
	{
		pass p0 { PixelShader = compile ps_2_0 DiffColorPass(); }
	}
