
# simple-cypher 

![](https://img.shields.io/badge/-C++-blue?logo=cplusplus)
![](https://img.shields.io/badge/license-MIT-blue)
![](https://img.shields.io/badge/version-1.0.0-orange)

simple-cypher は Xorlshift とバーナム暗号を組み合わせたシンプルな暗号機能を提供します。

提供される暗号機能は次のような特徴を有しています。

* ストリーム暗号です。
* コストの低い演算が使われているので比較的高速に動作します。
* 共通鍵暗号方式を採用しています。
* 望むならば暗号化にあたって追加のメモリ領域を必要としません(In-place)。
* 暗号化・復号の処理が同じなので、同じデータを２度暗号化すると、元の平文に戻ります。

提供される暗号機能で留意すべき点は次のとおりです。

* 各バイトの位置はそのままなので、第二次世界大戦時のドイツ軍の暗号文のように、所定の位置に必ず出現する文章があると、そこを足掛かりに復号されてしまう危険性があります。(現代で例えるならば HTML の `<!DOCTYPE html>` GLSL の `#version 300 es` などが挙げられます)
* 当然ですが、同梱されている鍵を抜き取られてしまうと、正規の手順で復号されてしまいます。

## Usage

ビルド後に作成される `dist/include` `dist/lib` 内のファイルを適切な場所に配置して使います。
それ以外は標準的なC言語ライブラリと同じように扱います。

```c
#include <stdio.h>
#include <simple-cypher/simple-cypher.h>

#define PLAINTEXT "Hello!"

int main (){

  char encrypted[sizeof(PLAINTEXT)] = {0};
  simple_cypher cypher;
  simple_cypher_setup(0x123, &cypher);
  simple_cypher_encrypt_decrypt(PLAINTEXT, sizeof(PLAINTEXT) -1, &cypher, encrypted);
  printf("encrypted=%s\n", encrypted);
  
  char decrypted[sizeof(PLAINTEXT)] = {0};
  simple_cypher cypher2;
  simple_cypher_setup(0x123, &cypher2);
  simple_cypher_encrypt_decrypt(encrypted, sizeof(encrypted) -1, &cypher2, decrypted);
  printf("decrypted=%s\n", decrypted); 
  
  return 0;

}
```

```bash
gcc -Idist/include -Ldist/lib example.c -lsimplecypher
```

## Command 

ビルド後に作成される `bin/simple-cypher.exe` を実行することで、コマンドラインから暗号機能を利用することができます。

```bash
./bin/simple-cypher -k 0x123
```

## Build 

同梱されている `makefile` を使用することでビルドすることができます。

```bash
make
```

非常に簡素なテストも同梱されています。

```bash
make test
```

## License 

&copy; 2024 tikubonn

simple-cypher released under the [MIT License](./LICENSE).
