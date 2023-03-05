// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
	// Add your code here . . .
	zSTRING zCParser::GetScriptString( zSTRING const& str, int index ) {
		zSTRING string;
		zCPar_Symbol* ps = GetSymbol( str );
		if ( ps )
			ps->GetValue( string, index );
		if ( string.Length() )
			return string;
		else
			return "";
		return "";
	};

	void zCParser::SetScriptString( zSTRING name, zSTRING str, int index ) {
		zCPar_Symbol* ps = GetSymbol( name );
		if ( ps )
			ps->SetValue( str, index );
	};

	int zCParser::GetScriptInt( zSTRING const& str, int index ) {
		int retVal = 0;
		zCPar_Symbol* ps = GetSymbol( str );
		if ( ps )
			ps->GetValue( retVal, index );
		else
			return -1;
		return retVal;

	};

	void zCParser::SetScriptInt( zSTRING name, int value, int index ) {
		zCPar_Symbol* sym = GetSymbol( name );

		if ( sym ) {

			sym->SetValue( value, index );
		}
	}

	void HandleResultString(zSTRING) zCall( 0x00432EC0 );

	unsigned char __cdecl GetCharFromDIK( int dik1, int dik2, int dik3 ) zCall( 0x004D2130 );

	unsigned char GetCharFromDIK_AST( int dik1, int dik2, int dik3 ) {
		return GetCharFromDIK( dik1, dik2, dik3 );
	}

	bool is_number( char* s ) {

		bool flag = true;

		for ( size_t i = 0; i < strlen( s ); i++ ) {
			if ( !isdigit( s[ i ] ) ) {
				flag = false;
				break;
			}
		}

		return flag;
	}
}