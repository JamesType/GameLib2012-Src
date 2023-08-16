# 「ゲームプログラマになる前に覚えておきたい技術」のソースコード(GameLib2012)をVisual Studio 2022に対応
* [公式対応](https://www.shuwasystem.co.jp/support/7980html/2118.html)とは違うもの
* Visual Studio 2022対応(sln, vcxproj, props)
* CharacterSetをUnicodeに変更
* ソースコードファイルをUTF-8(BOM)としてエンコード

## この中にあるものについて
この中にはVisual Studio 2022用のサンプルコードその他が入っています。  
ほとんどのディレクトリは各章向けのライブラリですが、そうでないものもあります。

* Configurations  
.vcxprojで利用されるプロパティシート。

* NonFree  
大半のサンプルは著者の落書きや冗談で作った音データしか使っていませんが、  
NonFreeソリューション内はプロの絵描きが用意した素材を使っております。  
使用条件については詳細はこのファイルと同じところにある「LICENSE.md」をごらんください。

* Tools  
DDS画像を作るDdsConverter、アーカイブを作るArchiver、  
.xからこの本で使った3D画像フォーマットに変換するXFileConverter  
のソースコードが入っています。

* その他のフォルダー  
サンプルのソースコードが入っています。
