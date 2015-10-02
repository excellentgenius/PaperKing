
namespace effect{
	enum class EFFECTTYPE{
		EFFECT_A,
		EFFECT_B,
		EFFECT_C,
		EFFECT_D,
		EFFECT_E,
		EFFECT_F,
		EFFECT_G,
		EFFECT_H,
		EFFECT_MAX,
	};

	extern void LoadReses();
	extern void Step();
	extern void Draw();
	extern void Init();
	extern void Create(int x, int y, EFFECTTYPE type);
}
