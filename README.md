# Space-Invaders

Compile no Vscode com o comando:

```
Ctrl+Shift+B
```

Caso não de certo faça uma pasta na raiz do projeto como

```
.vscode\tasks.json
```

## tasks.json

```
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "Compilar com Make",
      "type": "shell",
      "command": "make clean; make",
      "group": {
        "kind": "build",
        "isDefault": true
      },
      "problemMatcher": ["$gcc"],
      "detail": "Compilação usando Makefile"
    }
  ]
}
```

### O comando precisa desse tsk.json.

---

### Compile:

Saida esperada:
```  
*  Executing task: make clean; make 

rm -rf output
rm -f *.o
mkdir -p output
g++ -std=c++17 -Wall -Iinclude -Isrc -I. main.cpp -o output/main.exe -mconsole -Llib -lBearLibTerminal
cp BearLibTerminal.dll output/
 *  Terminal will be reused by tasks, press any key to close it. 
```

---

Arquitetura State Machine

[Video](https://www.youtube.com/watch?v=ow_Lum-Agbs&ab_channel=Bitlytic)

Trabalho

[M3](https://onedrive.live.com/?redeem=aHR0cHM6Ly8xZHJ2Lm1zL2IvYy8xYmIzM2I2ZDE1MGRiOTBiL0VaUVJOTmp1RTBwS3M5dmxYX2lTdHNJQkRZeDFVMEgxcUNWS21GUlNTYk1TY3c%5FZT1PaEFrdGk&cid=1BB33B6D150DB90B&id=1BB33B6D150DB90B%21sd834119413ee4a4ab3dbe55ff892b6c2&parId=1BB33B6D150DB90B%2141485&o=OneUp)