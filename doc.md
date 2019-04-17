---
title: Folyézis
subtitle: Víz folyásának szimulálása program által
institute: Bolyai Farkas Elméleti Líceum
documentclass: report
author: Gardner Mark
lang: hu
...
#bevezető
##motiváció

Azért választottam ezt a témát, mert akartam egy valósághű földrajzi
tájképeket generáló programot. A térkép-generálás egy nagyon népszerű téma,
ahogyan a következő projektek is bizonyítják

###példák
![xmountains](images/xmountains.png)

![fracplanet](images/fracplanet.png)

![Azagaar fantasztikus térkép generátora](images/Azagaar.png)

![Donjon.bin.sh](images/donjon2.png)

Stb.

Ezek a generátorok magas minőségű térképeket tudnak generálni, de ezek nem valós földrajzi folyamatokat használnak.

Ehelyett, fraktálokat állítanak elő, amelyek hasonlóak a földrajzi folyamatok eredményeihez.

###Földrajzi folyamatok

A létező domborzati formákat földrajzi folyamatok sokasága állítja elő és alakítja át.

A jelentősebbek:

 * Tektonika
 * Vulkanizmus
 * Erózió
	* Víz által
	* Levegő áltat
 * Emberi tevékenységek
 * Földrengések, földcsuszamlások

####Modellezés

Ezek közül soknak nincsenek gyors eredményei. 

 * A tektonika, habár óriási eredményeket ér el (példa erre a Himalája), ez évezredekbe telik.

![kép a Himalájáról](images/himalayas.jpg)

 * A vulkanizmus gyorsabb, de így is évekbe telik hogy valami nagyobb forma létrejöjjön, és kicsi méretekben nem érdekes (a magma viszkozitása miatt nem alakulnak ki használható térképek)

![vulkanizmus távolról és közelről](images/volcano.jpg)

 * Habár az emberi tevékenységeket nehéz szimulálni, mégis van aki megpróbálta. Akkor is, az emberi tevékenység földrajzi vonatkozását modellezni képtelenség.

![dwarf fortress](images/df.jpg)

 * A földrengések, habár sokszor nagyon nagy változásokat hoznak a földrajzban, nem használhatóak egyedülállóan térképek készítésére. Földcsuszamlások pedig csak más folyamatokkal együtt jönnek létre.

![milyen térképet csinálnának egyedül a földrengések](images/foldrenges.jpg)

 * A víz által létrehozott erózió meglepően alkalmas kicsi méretben is, nagy méretben is való modellezéshez.

![A víz által létrehozott domborzati formák, kicsiben és nagyban](images/river.jpg)

Az eróziót első ránézésre könnyebben lehetne szimulálni a felsoroltak közül, de ehhez kellene egy pár dolog:

 * Vagy víz, vagy levegő folyás szükséges arra, hogy létezzen erózió
 * A folyadékok dinamikája az egyik legnehezebb tere a fizikának. Sokan akik értenek hozzá mondják, hogy a kvantummechanika könnyebb.
 	* A víznek két folyási módja van, de a levegőt nem lehet kétdimenziósra egyszerűsíteni, amíg a vizet igen. Mivel a víz gyorsabban készít érdekes domborzatot, ezt választottam.

![A víz két folyási módja, színnel jelölve a természet által](images/turbulence.jpg)

##A dolgozat

### A dolgozat célja

A dolgozat célja a víz (keveset) egyszerűsített folyását modellezze.
Csak két egyszerűsítés van:

 1. A víz csak egy rétegben tud terjedni, mivel a memóriában egy kétdimenziós mátrix tárolja az összes jellemzőjét minden cellának.
 2. A víz kétfelé folyási módjából csak az egyik van letárgyalva: A lamináris folyás

### A dolgozat által használt technológia

#### A fő program

A dolgozat fő része c++ nyelven van megírva. Ez a része a projektnek grafikusan jeleníti meg a szimulációt, három képet rajzolva ki minden ötven lépésnél.

![A kirajzolt képekre egy példa](saves/save_1000.bmp)

Ezek közül a bal oldali a víz és a föld abszolút, összeadott magassága. Ezt a típusú képet számos háromdimenziós ábrázoló program fel tudja használni, és ezert egy logikus kimeneti adatja a programnak.

A középső egy haszontalan, de szép kinézetű kép, amely az előzőhöz hasonló adatokat tartalmaz, de a víz jelenlétet hangsúlyozza, és így más programnak majdnem érthetetlen. Lehetne a programnak egy olyan továbbfejlesztése, amely ehelyett a víz sebességét, esetleg folyási irányát mutatná.

A jobboldali kép a víz mélységet mutatja be, egy vöröses-barna háttéren, amely lehetne jobb is, de a vízen van a hangsúly, tehet mindegy. Ez a masik hasznalt kep, a haromdimenzos animacional.

A grafikat az `SDL-bgi` nevu grafikai konyvtar segitsegevel oldottam meg, amely [itt](http://libxbgi.sourceforge.net/) talalhato meg.

#### Az animacio

A haromdimenzios animacio a POV-ray nevu fotorealisztikus 