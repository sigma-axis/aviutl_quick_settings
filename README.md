# クイック設定 AviUtl プラグイン

AviUtl や拡張編集の環境設定の一部項目を別ウィンドウに表示して，手軽に手早く切り替えられるようになるプラグインです．環境設定のダイアログを表示させることなく，編集状況に応じて都合のいい設定にその都度切り替えられます．

一部の設定項目はショートカットキーによる設定切り替えができるよう，メニュー項目にも切り替えコマンドを追加しています．

[[ダウンロードはこちら]](https://github.com/sigma-axis/aviutl_quick_settings/releases)

![クイック設定のウィンドウ](https://github.com/user-attachments/assets/8cac328a-99be-4491-80f5-c771286faed6)

ウィンドウに表示させる項目は取捨選択できて，隠したり並び替えたりなど[カスタマイズ](#設定ファイル)ができます．


## 動作要件

- AviUtl 1.10 + 拡張編集 0.92

  http://spring-fragrance.mints.ne.jp/aviutl
  - 拡張編集 0.93rc1 等の他バージョンでは動作しません．

- Visual C++ 再頒布可能パッケージ（\[2015/2017/2019/2022\] の x86 対応版が必要）

  https://learn.microsoft.com/ja-jp/cpp/windows/latest-supported-vc-redist

## 導入方法

以下のフォルダのいずれかに `quick_settings.auf` と `quick_settings.json` をコピーしてください．

1. `aviutl.exe` のあるフォルダ
1. (1) のフォルダにある `plugins` フォルダ
1. (2) のフォルダにある任意のフォルダ


## 使い方

AviUtlメインウィンドウの「設定」メニューから「クイック設定の設定」を選択してウィンドウを表示させてください．

AviUtl や拡張編集の環境設定のうち，以下の項目をこのウィンドウから操作できます．

- AviUtl 本体の環境設定
  1.  任意フレーム数移動
  1.  編集のレジューム機能を有効
  1.  関連ウィンドウ同士を移動時にスナップする
  1.  トラックバーでクリックした位置に直接移動する

- 拡張編集の環境設定
  1.  編集画面の時間表示単位
  1.  設定ダイアログの時間表示単位
  1.  レイヤーの幅
  1.  再生ウィンドウで再生した時にカーソルを連動
  1.  カーソル移動時に自動でスクロール
  1.  D & D読み込み時に複数オブジェクトをグループ化
  1.  ファイルの場所が変更された時にプロジェクトパスから読む
  1.  中間点追加・分割時に中間の値を反映する
  1.  中間点追加・分割を常に現在フレームで行う
  1.  オブジェクトの端をつまむ時に隣接するオブジェクトも選択
  1.  分割時にグループを別々にする
  1.  RGB色空間で飽和
  1.  自動バックアップを有効
      - バックアップの時間間隔やファイルの最大数は，このウィンドウから変更できません．


### メニューコマンドとショートカットキー

一部の設定項目はメニューで切り替えコマンドが追加されています．ショートカットキーなどに割り当てて使用できます．

![追加メニュー](https://github.com/user-attachments/assets/0529f081-1781-4374-8941-3244f7c3ecf8)

次の項目は，複数ある設定値を順繰りに変更します:

- 拡張編集の設定
  1.  編集画面の時間表示単位
  1.  設定ダイアログの時間表示単位
  1.  レイヤーの幅

次の項目は，切り替えコマンドがありません:

- AviUtl 本体の環境設定
  1.  任意フレーム数移動


## 設定ファイル

導入時にコピーしてきたファイル `quick_settings.json` をテキストエディタで編集することで動作をカスタマイズできます．変更内容は AviUtl 再起動時に反映されます．

以下が可能です:
- 不必要な項目を非表示にする．
- 項目の順序を変更する．
- 項目間に余白やセパレータを配置する．
- 一部項目を押しボタン形式で表示する．

  ![通常形式とプッシュボタン形式](https://github.com/user-attachments/assets/15d57d5a-0ce9-4eb7-85a6-976772d16c93)

  通常形式とプッシュボタン形式の比較．

- ウィンドウ全体の幅や余白を調整する．

### `metrics`

ウィンドウ全体のレイアウトに影響する長さを指定します．

- `max_width`

  表示項目1つ当たりに許容する最大幅をピクセル単位で指定します．`0` だと幅の制約なしにレイアウトします．

  `0` でない場合の最小値は `128`, 初期値は `0`.

- `margin_h`, `margin_v`

  ウィンドウ端の上下左右に設定する余白をピクセル単位で指定します．

  - `margin_h` で左右の余白，`margin_v` で上下の余白です．

  最小値は `0`, 最大値は `32`, 初期値は `margin_h` が `8`, `margin_v` が `6` です．

- `gap_v`

  表示する各設定項目の間の余白をピクセル単位で指定します．

  最小値は `0`, 最大値は `32` 初期値は `2` です．


### `gui_items`

ウィンドウに表示する設定項目やセパレータなどを記述します．ここに並んだ順番通りにウィンドウ上に表示されます．

各項目ごとに `id` でどの設定項目かを指定します．`id` に応じて追加でオプションを指定できるものもあります．

| `id` | 設定項目 | 追加オプション |
|:---:|:---|:---|
| `frame_jump_distance` | 任意フレーム数移動 | (なし) |
| `resume_enabled` | 編集のレジューム機能を有効 | `push_like` |
| `snaps_window` | 関連ウィンドウ同士を移動時にスナップする | `push_like` |
| `seeks_clicked_point` | トラックバーでクリックした位置に直接移動する | `push_like` |
| `tl_time_format` | 編集画面の時間表示単位 | `push_like` |
| `dlg_time_format` | 設定ダイアログの時間表示単位 | `push_like` |
| `layer_size_mode` | レイヤーの幅 | `push_like` |
| `playback_syncs_cursor` | 再生ウィンドウで再生した時にカーソルを連動 | `push_like` |
| `scroll_follows_cursor` | カーソル移動時に自動でスクロール | `push_like` |
| `groups_drag_drop` | D & D読み込み時に複数オブジェクトをグループ化 | `push_like` |
| `relocates_file_path` | ファイルの場所が変更された時にプロジェクトパスから読む | `push_like` |
| `split_calcs_midvalue` | 中間点追加・分割時に中間の値を反映する | `push_like` |
| `splits_at_current` | 中間点追加・分割を常に現在フレームで行う | `push_like` |
| `drags_adjacent_obj` | オブジェクトの端をつまむ時に隣接するオブジェクトも選択 | `push_like` |
| `split_seps_groups` | 分割時にグループを別々にする | `push_like` |
| `clamps_into_rgb_box` | RGB色空間で飽和 | `push_like` |
| `auto_backup_enabled` | 自動バックアップを有効 | `push_like` |
| `----` or `====` | (水平線セパレータ) | `pad_h` `pad_v` |
| `    ` or `****` | (空白余白) | `pad_v` |

- `水平線セパレータ` の `id` 指定は，`-` や `=` から始まる任意の文字列で可能です．
- `空白余白` の `id` 指定は，` `(半角空白) や `*` から始まる任意の文字列で可能です．
- `水平線セパレータ` と `空白余白` 以外で同じ `id` が複数回現れた場合，2つ目以降は無視されます．同じ設定項目を複数箇所に表示させることはできません．

追加オプションの機能は以下の通りです:

| オプション名 | 有効な値 (デフォルト) | 内容 |
|:---:|:---:|:----|
| `push_like` | `true` / `false` **(`false`)** | チェックボックスやラジオボタンではなく，プッシュボタン形式で表示します． |
| `pad_h` | `0`--`32` **(`0`)** | `水平線セパレータ`の左右の余白を指定します．指定した長さだけ水平線が短くなります． |
| `pad_v` | `0`--`32` **(`0`)** | 上下の余白を指定します．指定した長さだけ上下にある項目との距離が開きます． |

> [!TIP]
> `id` の項目がなかったり，上記のどれとも一致しないものは無視されます．なので項目を非表示にしたい場合，その行ごと削除する他にも `id` を `!id` などに書き換えることでも可能です．


## 改版履歴

- **v1.00** (2024-08-28)

  - 初版．


## ライセンス

このプログラムの利用・改変・再頒布等に関しては MIT ライセンスに従うものとします．

---

The MIT License (MIT)

Copyright (C) 2024 sigma-axis

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

https://mit-license.org/


#  Credits

##  aviutl_exedit_sdk

https://github.com/ePi5131/aviutl_exedit_sdk

---

1条項BSD

Copyright (c) 2022
ePi All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
THIS SOFTWARE IS PROVIDED BY ePi “AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL ePi BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#  連絡・バグ報告

- GitHub: https://github.com/sigma-axis
- Twitter: https://x.com/sigma_axis
- nicovideo: https://www.nicovideo.jp/user/51492481
- Misskey.io: https://misskey.io/@sigma_axis
- Bluesky: https://bsky.app/profile/sigma-axis.bsky.social
