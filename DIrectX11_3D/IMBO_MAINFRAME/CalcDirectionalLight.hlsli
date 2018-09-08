

//ambient
cbuffer cbPsCameraStatic : register(b11) {
	float4x4 CameraProjectionMtx : packoffset(c0);
	float4 PerspectiveValues : packoffset(c4);
}
cbuffer cbPsCameraDynamic : register(b12) {
	float4x4 ViewInv : packoffset(c0);
	float4 EyePosition : packoffset(c4);
}
//ambient
