// Supported with union (c) 2020 Union team

// User API for zCParser
// Add your methods here

zSTRING zCParser::GetScriptString( zSTRING const& str, int index = 0 );
void zCParser::SetScriptString( zSTRING name, zSTRING str, int index = 0 );
void zCParser::SetScriptInt( zSTRING name, int value, int index = 0 );
int zCParser::GetScriptInt( zSTRING const& str, int index = 0 );