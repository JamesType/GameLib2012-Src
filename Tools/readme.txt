* ConvertToCode.exeはGameLibsのプロジェクトに利用されるので下記の対応が必要
  * プロジェクト「ConvertToCode」を実行してConvertToCode.exeを生成する
  * Toolsの直下にbinフォルダーを新規追加する
  * 生成されたConvertToCode.exeをbinフォルダーに入れる

* Archiver.vcxproj
  - app.props
  - appCnsl.props
  - OnlyExplicitInline
  - $(GAME_LIB_DIR)\Final\include;$(ProjectDir);
  - GameLib_d.lib;
  - GameLib.lib;
  - $(GAME_LIB_DIR)\Final\lib;

* ConvertToCode.vcxproj
  - app.props
  - appCnsl.props
  - OnlyExplicitInline

* PrimeNumberGenerator.vcxproj
  - app.props
  - appCnsl.props
  - OnlyExplicitInline
  - MultiThreadedDebugDLL
  - MultiThreadedDLL

* PseudoXmlConverter.vcxproj
  - app.props
  - appCnsl.props
  - OnlyExplicitInline
  - $(GAME_LIB_DIR)\Final\include;$(ProjectDir);
  - GameLib_d.lib;
  - GameLib.lib;
  - $(GAME_LIB_DIR)\Final\lib;

* XFileConverter.vcxproj
  - app.props
  - appCnsl.props
  - OnlyExplicitInline
  - $(GAME_LIB_DIR)\Final\include;$(ProjectDir);
  - GameLib_d.lib;
  - GameLib.lib;
  - $(GAME_LIB_DIR)\Final\lib;