
struct GS_INPUT
{
    float4 position : SV_POSITION;  // ���_
    float  edge     : COLOR;
};

struct GS_OUTPUT
{
    float4 position : SV_POSITION;  // ���_
    float  edge     : COLOR;
};

// �W�I���g���V�F�[�_�ŗ֊s������A���C���o��
[maxvertexcount(10)]
void main(
	lineadj GS_INPUT input[4],
    uint primitiveID : SV_PrimitiveID,
	inout LineStream<GS_OUTPUT> stream
)
{
    // w�Ŋ����Ēʏ�̍��W�n�ɖ߂����Ƃ�Y�ꂸ��
    float3 v0 = input[1].position.xyz / input[1].position.w;
    float3 v1 = input[2].position.xyz / input[2].position.w;
    float3 va = input[0].position.xyz / input[0].position.w;
    float3 vb = input[3].position.xyz / input[3].position.w;

    //�|���S�� v0-v1-va��v1-v0-vb�̕����𒲂׃��C���`��	
    float3 v0a = v0 - va;
    float3 v1a = v1 - va;
    float3 v0b = v0 - vb;
    float3 v1b = v1 - vb;

    if (0 > cross(v0a, v1a).z * cross(v1b, v0b).z) {
        GS_OUTPUT vo0 = (GS_OUTPUT)0;
        GS_OUTPUT vo1 = (GS_OUTPUT)0;
        float4 p0 = input[1].position;
        float4 p1 = input[2].position;

        vo0.position = p0;
        vo0.edge = input[1].edge;

        vo1.position = p1;
        vo1.edge = input[2].edge;

        stream.Append(vo0);
        stream.Append(vo1);
        stream.RestartStrip();

        //��{���ƍׂ��̂ŏ㉺���E�ɂ��炵�ĕ`��
        const float px = 0.8*p0.w / 640.0;
        const float py = 0.8*p1.w / 480.0;

        vo0.position = p0 + float4(px, 0, 0, 0);
        vo1.position = p1 + float4(px, 0, 0, 0);
        stream.Append(vo0);
        stream.Append(vo1);
        stream.RestartStrip();

        vo0.position = p0 + float4(-px, 0, 0, 0);
        vo1.position = p1 + float4(-px, 0, 0, 0);
        stream.Append(vo0);
        stream.Append(vo1);
        stream.RestartStrip();

        vo0.position = p0 + float4(0, py, 0, 0);
        vo1.position = p1 + float4(0, py, 0, 0);
        stream.Append(vo0);
        stream.Append(vo1);
        stream.RestartStrip();

        vo0.position = p0 + float4(0, -py, 0, 0);
        vo1.position = p1 + float4(0, -py, 0, 0);
        stream.Append(vo0);
        stream.Append(vo1);
        stream.RestartStrip();
    }
}