/* Vertex 속성들. Vertex Shader 의 Input 값으로 받게된다 */
struct vs_in {
	float3 position_local : POS;
	float3 color : COL;
};

/* 
	Vertex Shader 의 결과값. Pixel Shader 의 Input 값임. 보통 Pixel Shader 로 오기전 
	Rasterizer 에 의해 보간 (Interpolation) 처리 됨  
*/
struct vs_out {
	float4 position_clip : SV_POSITION;
};

vs_out vs_main(vs_in input) {
	vs_out output = (vs_out)0; // zero the memory first
	output.position_clip = float4(input.position_local, 1.0);
	return output;
}

float4 ps_main(vs_out input) : SV_TARGET {
	return float4( 1.0, 0.0, 0.0, 1.0 ); 
}