# Introduction #

Glide64 uses [GNU gettext](http://www.gnu.org/software/gettext/) approach for internalization: translations are stored in message catalog files, which are loaded by an application. There are two kinds of message catalogs: source catalogs which are text files with extension .po and binary catalogs which are generated from the source and have the extension .mo. Only the binary files are needed during program execution. Glide64 dynamically builds list of available translation files, so new translation will be available right after corresponding translation file is copied into proper place.

## How to create translation file ##

  1. To edit message catalog files you need special program, [poEdit](http://www.poedit.net/). It exists for nearly every popular platform, has simple and continent interface and good user guide.
  1. You have to name your message catalog properly. Glide64 selects needed locale by message catalog’s file name. The file name must be as follow:<br><code>Glide64_[canonical locale name].po</code>, e.g. Glide64_fr_FR.po<br>Canonical locale name is canonical form of current locale name. Canonical form is the one that is used on UNIX systems: it is a two- or five-letter string in xx or xx_YY format, where xx is ISO 639 code of language and YY is ISO 3166 code of the country. Examples are "en", "en_GB", "en_US" or "fr_FR". If you don’t know canonical form for your language, load and run <a href='http://glide64.emuxhaven.net/files/internat.zip'>this sample program</a> and open File->About. Here you will find information about canonical locale name for your system locale.<br>
<ol><li>There is master message catalog named <b>Glide64en.pot</b> It is a text file, automatically generated from Glide64 sources. It contains only lines, need to be translated, and no translations. To create new translation file, download recent Glide64en.pot from <a href='http://code.google.com/p/glidehqplusglitch64/source/browse/trunk/Glide64/Internalization/Glide64_en.pot'>sources</a>, load it into <a href='http://www.poedit.net/'>poEdit</a>, select <i>File->SaveAs</i> and give the proper name for your translation file. All you need to do then is to translate all messages to your language. When you save your work, corresponding binary .mo file is automatically generated, so you may test, how it works. There are few special things:<br>
<ul><li>There is a special string: <i>LANGUAGE_NAME</i>. Its translation must by native name of you language, like <i>Français</i>, <i>Deutsch</i>. This name will be shown in the “Language” button text. If this string is not translated, english name for selected language will be used.<br>
</li><li>Some lines contain special combinations: \n, \”<br>
<ul><li>\n inserts line break in the text.<br>
</li><li>\” inserts quotes in the text.<br>
</li></ul></li></ul><blockquote>If original line contain such combination(s), it’s better to put them into translated line too. Don’t put quotes without ‘\’ character.<br>
</blockquote><ul><li>Some lines have space character at the end. It is done for purpose, and translated line also must have space at the end.<br>
</li><li>Translated text can be larger than original one. E.g. for Russian it’s usually true. Some labels on GUI panels may look ugly after translation. You may add \n to the translation line to split it in two, even when original line doesn’t have it. This works only for labels; checkbox descriptions can’t be split.<br>
</li><li>When you save your translation, poEdit may say something like: “msgfmt: found fatal error”. Usually it can be ignored: msgfmt does not like ‘%’ character in the output string.</li></ul></li></ol>


<h2>How to use translation file</h2>

When translation is done, copy binary message catalog file (.mo) into the proper place. It is Plugin folder for Windows, and config folder for Linux. After that open Glide64 configuration dialog and on the Basic Settings panel press ‘Language” button. List of available translations will be opened. Selected language will be activated after restart of the configuration dialog.<br>
<br>
<h2>How to update translation file</h2>

<ol><li>If you noticed typos or mistakes in the translation, open corresponding .po file with poEdit, correct the problem and save your work. New .mo file will be generated and ready to use.<br>
</li><li>If some text was changed in the program sources, all translations files will have to be updated. To do that, new master catalog file Glide64en.pot will be generated. To update your translation catalog, load it with poEdit, select Catalog->Update from POT file, select Glide64en.pot and press OK. All newly added string will have empty translation, all removed strings will disappear, and all changed strings will have status "Fuzzy translation". You will need to correct "fuzzy" strings, translate new ones and save your work.</li></ol>

<h2>How to submit your translation</h2>

Send your work to me by email, or post it here: <a href='http://code.google.com/p/glidehqplusglitch64/issues/detail?id=61'>http://code.google.com/p/glidehqplusglitch64/issues/detail?id=61</a>

<h2>Where to get translation for your language</h2>

Master message catalog <b>Glide64en.pot</b> and translated message catalogs in text form (.po) are stored in the <a href='http://code.google.com/p/glidehqplusglitch64/source/browse/#svn/trunk'>Glide64 repository</a>. You may download latest version from <a href='http://code.google.com/p/glidehqplusglitch64/source/browse/#svn/trunk/Glide64/Internalization'>here</a>, load into poEdit, create .mo file, copy it into proper place and enjoy. You may also use msgfmt program to create .mo file. Glide64 releases contain all translations, available at the release time.