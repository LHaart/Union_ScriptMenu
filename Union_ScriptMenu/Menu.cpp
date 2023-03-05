// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
	// Add your code here . . .
	ScriptMenu sMenu;
	HOOK ivk_oCNpc_OpenScreen_Status AS( &oCNpc::OpenScreen_Status, &oCNpc::OpenScreen_Status_Union );
	void oCNpc::OpenScreen_Status_Union() {
		sMenu.Enable();
	}

	HOOK ivk_oCGame_HandleEvent AS( &oCGame::HandleEvent, &oCGame::HandleEvent_Union );
	zBOOL oCGame :: HandleEvent_Union (int key) {
		if(sMenu.enabled) {
			zWORD logicalKey = zinput->GetFirstBindedLogicalKey(key);

			if ((key != KEY_ESCAPE) && (logicalKey != GAME_END) && logicalKey != GAME_SCREEN_STATUS)
			{
				unsigned char ch = GetCharFromDIK_AST(key, 0, 0);
				if((ch >= 32) && (ch <= 255)) {
					char* str = new char[2];
					str[0] = static_cast<char>(ch);
					str[1] = '\0';
					sMenu.resultStr += str;
				}
					//sMenu.resultStr = Z ( (sMenu.resultStr) + (char)(ch));
			}

			if(logicalKey == GAME_SCREEN_LOG || logicalKey == GAME_SCREEN_MAP) {
				key = KEY_LSHIFT;
			}
			
		}

		return THISCALL( ivk_oCGame_HandleEvent ) (key);
		
	}


	void ScriptMenu::Init() {
		enabled = 0;
		page = 1;
		debugEnabled = 0;

		mainView = zNEW( zCView )(0, 0, 8192, 8192);
		back = zNEW( zCView );
		text = zNEW( zCView )(0, 0, 8192, 8192);
		debugView = zNEW( zCView )(0, 0, 8192, 8192);

		mainView->InsertItem(back);
		mainView->InsertItem(debugView);
		back->InsertItem(text);
	}

	void ScriptMenu::Parse() {
		screen->InsertItem(mainView);
		screen->RemoveItem(mainView);
		mainView->ClrPrintwin();
		debugEnabled = parser->GetScriptInt("ScriptMenu_DebugEnabled");
		enabled = true;

		// парсинг MENU_MAIN
		static zSTRING mainBack = parser->GetScriptString("ScriptMenuMainBack");
		zSTRING backPage = mainBack + "_" + Z page;
		zSTRING sBack = parser->GetScriptString(backPage, 0);
		int sizeX = GetInt(backPage, 1);
		sizeX = screen->anx( sizeX );
		int sizeY = GetInt(backPage, 2);
		sizeY = screen->any( sizeY );
		int posX = GetInt(backPage, 3);
		int posY = GetInt(backPage, 4);
		
		back->InsertBack(sBack);
		back->SetSize( sizeX, sizeY );

		if(posX < 0 || posY < 0) {
			posX = (8192 - sizeX) / 2;
			posY = (8192 - sizeY) / 2;
			back->SetPos(posX, posY);
		} else {
			back->SetPos(posX, posY);
		}
		back->RemoveItem( text );
		back->InsertItem( text );

		// парсинг MENU_MAIN
		int count = 1;
		zSTRING arrBaseName = parser->GetScriptString( "ScriptMenuVarName" );
		text->ClrPrintwin();
		while(true) {
			zSTRING arrName = arrBaseName + "_" + Z page + "_" + Z count;
			zSTRING par = parser->GetScriptString(arrName, 0);
			if(par.IsEmpty()) break;

			zSTRING txt = "";
			int x = -1, y = -1;
			GetElement(arrName, 0, txt, x, y);
			text->Print( x, y, txt );

			zSTRING par2 = parser->GetScriptString(arrName, 5);
			if(!par2.IsEmpty()) {
				zSTRING txt2 = "";
				GetElement(arrName, 1, txt2, x, y);
				text->Print( x, y, txt2 );
			}

			count += 1;
		}
		

		// вывод номера страницы.
		int backX = back->vposx;
		int backY = back->vposy;
		int backSizeX = back->vsizex;
		int backSizeY = back->vsizey;

		int centerX = backX + backSizeX / 2;
		
		static zSTRING pgMain = parser->GetScriptString("ScriptMenuPageName");
		static int pgNum = parser->GetScriptInt("ScriptMenu_Pages");
		zSTRING pg = pgMain + Z page;

		int x = centerX - mainView->FontSize( pg ) / 2;
		int y = backY + backSizeY + mainView->FontY() * 1;

		mainView->Print( x, y, pg );

		if(page < pgNum) {
			zSTRING right = " >>>";
			int pX = x + mainView->FontSize( pg );
			mainView->Print( pX, y, right );
		}

		if(page > 1) {
			zSTRING left = "<<< ";
			int pX = x - mainView->FontSize( left );
			mainView->Print( pX, y, left );
		}

		screen->InsertItem(mainView);
	}

	void ScriptMenu::Debug() {
		if(enabled && debugEnabled) {

			debugView->Print( 0, 0, "Дебаг меню активен." );
			debugView->Print( 0, 200 * 1, "Страница " + Z page );
			debugView->Print( 0, 200 * 2, "Стр+ Home, Стр- End" );
			static int numElement = 1;
			static int element = 1;
			static int debX = 0;
			static int debY = 0;
			static int isValue = 0;
			zSTRING sNumElement = (numElement == 1 ? parser->GetScriptString( "ScriptMenuVarName" ) : parser->GetScriptString( "ScriptMenuIconName" ));
			debugView->Print( 0, 200 * 3, "Текущий выбор: " + sNumElement );
			debugView->Print( 0, 200 * 4, "След. выбор: DELETE" );
			zSTRING sElement = sNumElement + "_" + Z page + "_" + Z element;
			debugView->Print( 0, 200 * 5, "Текущий выбранный элемент:" + sElement);
			debugView->Print( 0, 200 * 6, "Пред. элем: PageDown, След элем: PageUp" );
			debugView->Print( 0, 200 * 7, "Выбран текст или значение?: " + Z (isValue == 0 ? "Текст" : "Значение") );
			debugView->Print( 0, 200 * 8, "Сменить текст/значение: INSERT");
			debugView->Print( 0, 200 * 9, "Текущий элемент: " + GetDebugElement(sElement, isValue));
			debugView->Print( 0, 200 * 10, "Изменить положение: WASD ");


			if(zKeyPressed( KEY_INSERT )) {
				zinput->ClearKeyBuffer();
				isValue = isValue > 0 ? 0 : 1;
			}

			if(zKeyPressed( KEY_PGUP )) {
				zinput->ClearKeyBuffer();
				element += 1;
			}

			if(zKeyPressed( KEY_PGDN )) {
				zinput->ClearKeyBuffer();
				element -= 1;
			}


			if(zKeyPressed( KEY_DELETE )) {
				zinput->ClearKeyBuffer();
				numElement = numElement == 1 ? 2 : 1;
			}

			if(zKeyPressed( KEY_END )) {
				zinput->ClearKeyBuffer();
				page--;
			}

			if(zKeyPressed( KEY_HOME )) {
				zinput->ClearKeyBuffer();
				page++;
			}

			if(zKeyPressed( KEY_A )) {
				zSTRING text = "";
				int x = -1, y = -1;
				GetElement(sElement, isValue, text, x, y);
				if(x >= 0 && y >= 0) {
					x -= 1;
					if(zKeyPressed( KEY_LSHIFT ))  x -= 9;
					SetElementXY( sElement, isValue, x, y );
					Parse();
				}
			}

			if(zKeyPressed( KEY_D )) {
				zSTRING text = "";
				int x = -1, y = -1;
				GetElement(sElement, isValue, text, x, y);
				if(x >= 0 && y >= 0) {
					x += 1;
					if(zKeyPressed( KEY_LSHIFT ))  x += 9;
					SetElementXY( sElement, isValue, x, y );
					Parse();
				}
			}

			if(zKeyPressed( KEY_W )) {
				zSTRING text = "";
				int x = -1, y = -1;
				GetElement(sElement, isValue, text, x, y);
				if(x >= 0 && y >= 0) {
					y -= 1;
					if(zKeyPressed( KEY_LSHIFT ))  y -= 9;
					SetElementXY( sElement, isValue, x, y );
					Parse();
				}
			}

			if(zKeyPressed( KEY_S )) {
				zSTRING text = "";
				int x = -1, y = -1;
				GetElement(sElement, isValue, text, x, y);
				if(x >= 0 && y >= 0) {
					y += 1;
					if(zKeyPressed( KEY_LSHIFT ))  y += 9;
					SetElementXY( sElement, isValue, x, y );
					Parse();
				}
			}
		}
	}

	void ScriptMenu::SetElementXY( zSTRING elem, int isValue, int x, int y ) {
		if(!isValue) {
			parser->SetScriptString(elem, x, 3);
			parser->SetScriptString(elem, y, 4);
		} else {
			parser->SetScriptString(elem, x, 8);
			parser->SetScriptString(elem, y, 9);
		}
	}

	void ScriptMenu::GetElement( zSTRING elem, int isValue, zSTRING &text, int &x, int &y ) {
		if(!isValue) {
			zSTRING firstParam = parser->GetScriptString(elem, 0);

			if(firstParam == "TEXT") {
				text = parser->GetScriptString(elem, 1);
				x = GetInt(elem, 3);
				y = GetInt(elem, 4);
			} else if (firstParam == "ARRAY" || firstParam == "ARRAY_SUBSTR") { // просто для того, чтобы в скриптах различать, что для чего
				zSTRING sText = parser->GetScriptString(elem, 1);
				zSTRING p = parser->GetScriptString( elem, 2 );
				Array<CStringA> arr = (A p).Split( ' ' );
				int value = GetInt(elem, 2);

				if(arr.GetNum() > 2) {
					int index = arr.GetSafe( 2 )->ToInt32();
					zSTRING tText = parser->GetScriptString(sText, index);
					arr = (A tText).Split( '|' );
					if(arr.GetNum() > value) {
						text = Z(*arr.GetSafe( value ));
					}
					//ogame->game_text->Printwin("ARRAY " + sText + " -" + text + "- " + Z value + " " + tText );
				} else {
					text = parser->GetScriptString(sText, value);
				}
				
				
				x = GetInt(elem, 3);
				y = GetInt(elem, 4);
			} else if(firstParam == "PARAM") {
				text = Z GetInt(elem, 1);

				x = GetInt(elem, 3);
				y = GetInt(elem, 4);
			} else if(firstParam == "VAR INT") {
				zSTRING var = Z parser->GetScriptString( elem, 1 );
				text = Z parser->GetScriptInt( var );

				var = parser->GetScriptString( elem, 2 );
				if(!var.IsEmpty()) {
					zSTRING sText2 = Z parser->GetScriptInt( var );
					text += "/" + sText2;
				}
				x = GetInt(elem, 3);
				y = GetInt(elem, 4);
			} else if(firstParam == "VAR STR") {
				zSTRING var = Z parser->GetScriptString( elem, 1 );
				text = Z parser->GetScriptString( var );

				x = GetInt(elem, 3);
				y = GetInt(elem, 4);
			}
		} else {
			zSTRING firstParam = parser->GetScriptString(elem, 5);
			if(!firstParam.IsEmpty()) {
				if(firstParam == "TEXT") {
					text = parser->GetScriptString(elem, 6);
					zSTRING sText2 = parser->GetScriptString(elem, 7);
					if(!sText2.IsEmpty()) {
						text += "/" + sText2;
					}
					x = GetInt(elem, 8);
					y = GetInt(elem, 9);
				} else if(firstParam == "VAR INT") {
					zSTRING var = parser->GetScriptString( elem, 6 );
					text = Z parser->GetScriptInt( var );

					var = parser->GetScriptString( elem, 7 );
					if(!var.IsEmpty()) {
						zSTRING sText2 = Z parser->GetScriptInt( var );
						text += "/" + sText2;
					}
					x = GetInt(elem, 8);
					y = GetInt(elem, 9);
				} else if(firstParam == "VAR STR") {
					zSTRING var = Z parser->GetScriptString( elem, 6 );
					text = Z parser->GetScriptString( var );

					x = GetInt(elem, 8);
					y = GetInt(elem, 9);
				} else if(firstParam == "PARAM") {
					text = Z GetInt(elem, 6);
					zSTRING sText2 = Z GetInt(elem, 7);
					if(!sText2.IsEmpty() && sText2 != "-1" ) {
						text += "/" + sText2;
					}
					x = GetInt(elem, 8);
					y = GetInt(elem, 9);
				} else if(firstParam == "END") {
					zSTRING text2 = "";
					int x2 = -1;
					int y2 = -1;
					GetElement( elem, !isValue, text2, x2, y2 );
					text = parser->GetScriptString(elem, 6);
					x += this->text->FontSize(text2);
				}
			}
		}
	}

	zSTRING ScriptMenu::GetDebugElement( zSTRING elem, int isValue ) {
		zSTRING text = "";
		int x = -1, y = -1;
		GetElement(elem, isValue, text, x, y);
		return text + " x: " + Z x + ", y: " + Z y;
	}

	void ScriptMenu::Enable() {
		if(enabled) {
			enabled = false;
			debugEnabled = false;
			screen->RemoveItem(mainView);
			if (ogame) ogame->Unpause();
			player->movlock = false;	
			ogame->SetShowPlayerStatus(TRUE);
			HandleResultString(resultStr);
		} else {
			zBOOL oldShow = ogame->GetShowPlayerStatus();
			ogame->SetShowPlayerStatus(FALSE);
			Parse();
			if (ogame) ogame->Pause(0);
			player->movlock = true;
			resultStr.Clear();
		}
	}

	void ScriptMenu::Loop() {
		

		if(enabled) {
			if(!ogame->singleStep) {
				Enable();
				return;
			}

			if(ogame->singleStep && zKeyToggled( KEY_ESCAPE )) {
				zinput->ClearKeyBuffer();
				Enable();
				return;
			}

			int countPages = parser->GetScriptInt("ScriptMenu_Pages");
			if(countPages > 1) {
				if(page < countPages && zKeyToggled(KEY_RIGHT)) {
					page++;
					Parse();
				}

				if(page > 1 && zKeyToggled(KEY_LEFT)) {
					page--;
					Parse();
				}
			}
		}

		if(ogame->game_testmode || zCConsole::cur_console) return;

		Debug();
	}

	int ScriptMenu::GetInt( zSTRING param, int index ) {
		zSTRING p = parser->GetScriptString( param, index );
		if(!p.IsEmpty()) {
			int var = 0;
			if(p == "player.guild") {
				var = player->guild;
			} else if(p == "player.lp") {
				var = player->learn_points;
			} else if(p == "player.exp") {
				var = player->experience_points;
			} else if(p == "player.exp_next") {
				var = player->experience_points_next_level;
			} else if(p == "player.level") {
				var = player->level;
			} else if((A p).HasWord("player.talent")) {
				Array<CStringA> arr = (A p).Split( ' ' );
				if(arr.GetNum() > 1 && arr.GetSafe( 1 )) {
					int index = arr.GetSafe( 1 )->ToInt32();
					var = player->GetTalentSkill( index );
				}
			} else if((A p).HasWord("player.attribute")) {
				Array<CStringA> arr = (A p).Split( ' ' );
				if(arr.GetNum() > 1 && arr.GetSafe( 1 )) {
					int index = arr.GetSafe( 1 )->ToInt32();
					var = player->attribute[index];
				}
			} else if((A p).HasWord("player.protection")) {
				Array<CStringA> arr = (A p).Split( ' ' );
				if(arr.GetNum() > 1 && arr.GetSafe( 1 )) {
					int index = arr.GetSafe( 1 )->ToInt32();
					var = player->protection[index];
				}
			} else if((A p).HasWord("player.hitchance")) {
				Array<CStringA> arr = (A p).Split( ' ' );
				if(arr.GetNum() > 1 && arr.GetSafe( 1 )) {
					int index = arr.GetSafe( 1 )->ToInt32();
					var = player->hitChance[index];
				}
			} else {
				var = (A p).ToInt32();
			}
			
			return var;
		}

		return -1;
	}
}