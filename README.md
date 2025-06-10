# zip
要件 (Requirements)
このプログラムをコンパイルして実行するには、以下の環境が必要です。

Cコンパイラ (例: gcc, clang)
libzip ライブラリ
pkg-config (コンパイルを簡単にするため)
導入手順 (Setup)
1. 依存ライブラリのインストール
まず、お使いのオペレーティングシステムに応じて libzip と pkg-config をインストールします。

macOS (Homebrew):

Bash

brew install libzip pkg-config
Debian / Ubuntu:

Bash

sudo apt update
sudo apt install libzip-dev pkg-config
Fedora / CentOS / RHEL:

Bash

sudo dnf install libzip-devel pkg-config
2. コンパイル
依存ライブラリをインストールしたら、ターミナルで以下のコマンドを実行して zip_folder.c をコンパイルします。

Bash

gcc zip_folder.c $(pkg-config --cflags --libs libzip) -o zip_folder
コンパイルが成功すると、同じディレクトリに zip_folder という名前の実行ファイルが生成されます。

使い方 (Usage)
以下の構文でプログラムを実行します。

./zip_folder <出力するZIPファイル名> <圧縮するフォルダ名>
<出力するZIPファイル名>: 作成したいZIPファイルの名前を指定します (例: archive.zip)。
<圧縮するフォルダ名>: ZIPに圧縮したいフォルダのパスを指定します (例: my_folder)。
実行例
例えば、documents というフォルダを backup.zip という名前で圧縮したい場合は、以下のように実行します。

圧縮したいフォルダとファイルを用意します。

documents/
├── report.txt
└── images/
    └── photo.jpg
以下のコマンドを実行します。

Bash

./zip_folder backup.zip documents
実行すると、以下のようなログが出力されます。

'documents' をZIPファイル 'backup.zip' に圧縮中...
 -> ディレクトリ 'images/' を追加しました。
 -> ファイル 'images/photo.jpg' を追加しました。
 -> ファイル 'report.txt' を追加しました。
正常にZIPファイルを作成しました。
この結果、カレントディレクトリに backup.zip ファイルが生成されます。

