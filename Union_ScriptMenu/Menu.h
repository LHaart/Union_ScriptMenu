// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
	// Add your code here . . .
	class ScriptMenu {
	public:
		zCView* mainView;
		zCView* debugView;
		zCView* back;
		zCView* text;
		int enabled;
		int page;
		int debugEnabled;
		zSTRING resultStr;


		void Loop();
		void Init();
		int GetInt( zSTRING param, int index);
		int GetInt( zSTRING param);
		zSTRING GetDebugElement(zSTRING elem, int isValue);
		void GetElement( zSTRING elem, int isValue, zSTRING &text, int &x, int &y );
		void SetElementXY(zSTRING elem, int isValue, int x, int y);
		void Parse();
		void Debug();
		void Enable();
	};
}