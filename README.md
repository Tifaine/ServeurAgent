Programme serveur pour SMA

L'intérêt de ce logiciel est d'assurer l'enregistrement d'agents au sein d'un système et le transfert de messages entre les différentes entités du système.

Le serveur se base sur une logique "Publish/Subscribe". Les entités indiquent à quels topics ils veulent souscrire et publient sur un topic donné.

Pour notre application agents, chaque fois qu'un agent arrive dans le système, le serveur lui attribue un topic unique et un nom unique.
Quand un agent se déconnecte du système, le serveur efface son topic et prévient la plateforme de répartition des tâches.

Pour cela, il doit différencier le type d'entité qui se connecte, si c'est une plateforme ou un agent 'simple'.

## Le récupérer:
```Shell
> git clone --recurse-submodules https://github.com/Tifaine/ServeurAgent
```

## Compiler:
### Première foi:
```Shell
> ./Configure/configure
> make
```

### Après:
```Sehll
> make
```

## Utilisation:
Vous pouvez lancer le programme comme suit :
```Shell
> ./bin/exec
```