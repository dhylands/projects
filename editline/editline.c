<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en-US" lang="en-US">
<head>
<link rel="icon" href="/mozilla-central/static/hgicon.png" type="image/png" />
<meta name="robots" content="index, nofollow"/>
<link rel="stylesheet" href="/mozilla-central/static/style-gitweb.css" type="text/css" />

<style type="text/css">
div.feed {
  float: right;
}
a img {
  border-width: 0px;
}
div.log_link {
  width: 80px;
  background-color: white;
}

div.log_body {
  padding-left: 96px;
}
</style>

<title>mozilla-central: js/src/editline/editline.c@b2fd9044b65c</title>
<link rel="alternate" type="application/atom+xml"
   href="/mozilla-central/atom-log" title="Atom feed for mozilla-central"/>
</head>
<body>

<div class="page_header">
<a href="http://developer.mozilla.org/en/docs/Mercurial" title="Mercurial" style="float: right;">Mercurial</a><a href="/mozilla-central/summary">mozilla-central</a> - file revision - js/src/editline/editline.c@b2fd9044b65c
</div>

<div class="page_nav">
<a href="/mozilla-central/summary">summary</a> |
<a href="/mozilla-central/shortlog">shortlog</a> |
<a href="/mozilla-central/log">changelog</a> |
<a href="/mozilla-central/graph">graph</a> |
<a href="/mozilla-central/tags">tags</a> |
<a href="/mozilla-central/file/b2fd9044b65c/js/src/editline/">files</a> |
<a href="/mozilla-central/rev/b2fd9044b65c">changeset</a> |
file |
<a href="/mozilla-central/log/b2fd9044b65c/js/src/editline/editline.c">revisions</a> |
<a href="/mozilla-central/annotate/b2fd9044b65c/js/src/editline/editline.c">annotate</a> |
<a href="/mozilla-central/diff/b2fd9044b65c/js/src/editline/editline.c">diff</a> |
<a href="/mozilla-central/raw-file/b2fd9044b65c/js/src/editline/editline.c">raw</a><br/>
</div>

<div class="title">js/src/editline/editline.c</div>

<div class="title_text">
<table cellspacing="0">
<tr>
 <td>author</td>
 <td>&#68;&#63;&#111;&#32;&#71;&#111;&#116;&#116;&#119;&#97;&#108;&#100;&#32;&#60;&#100;&#97;&#111;&#64;&#109;&#111;&#122;&#105;&#108;&#108;&#97;&#46;&#99;&#111;&#109;&#62;</td></tr>
<tr>
 <td></td>
 <td>Sun Jan 10 13:28:49 2010 +0100 (at Sun Jan 10 13:28:49 2010 +0100)</td></tr>

<tr>
 <td>changeset 36979</td>
 <td style="font-family:monospace"><a class="list" href="/mozilla-central/rev/b2fd9044b65c">b2fd9044b65c</a></td></tr>
<tr><td>parent 18961</td><td style="font-family:monospace"><a class="list" href="/mozilla-central/file/48ca5b355829/js/src/editline/editline.c">48ca5b355829</a></td></tr>

<tr>
 <td>permissions</td>
 <td style="font-family:monospace">-rw-r--r--</td></tr>
</table>
</div>

<div class="page_path">
merge
</div>

<div class="page_body">
<div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1" id="l1">     1</a> /* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*-
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l2" id="l2">     2</a>  *
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l3" id="l3">     3</a>  * ***** BEGIN LICENSE BLOCK *****
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l4" id="l4">     4</a>  * Version: MPL 1.1/GPL 2.0/LGPL 2.1
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l5" id="l5">     5</a>  *
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l6" id="l6">     6</a>  * The contents of this file are subject to the Mozilla Public License Version
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l7" id="l7">     7</a>  * 1.1 (the &quot;License&quot;); you may not use this file except in compliance with
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l8" id="l8">     8</a>  * the License. You may obtain a copy of the License at
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l9" id="l9">     9</a>  * http://www.mozilla.org/MPL/
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l10" id="l10">    10</a>  *
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l11" id="l11">    11</a>  * Software distributed under the License is distributed on an &quot;AS IS&quot; basis,
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l12" id="l12">    12</a>  * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l13" id="l13">    13</a>  * for the specific language governing rights and limitations under the
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l14" id="l14">    14</a>  * License.
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l15" id="l15">    15</a>  *
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l16" id="l16">    16</a>  * The Original Code is Mozilla Communicator client code, released
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l17" id="l17">    17</a>  * March 31, 1998.
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l18" id="l18">    18</a>  *
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l19" id="l19">    19</a>  * The Initial Developer of the Original Code is
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l20" id="l20">    20</a>  * Simmule Turner and Rich Salz.
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l21" id="l21">    21</a>  * Portions created by the Initial Developer are Copyright (C) 1998
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l22" id="l22">    22</a>  * the Initial Developer. All Rights Reserved.
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l23" id="l23">    23</a>  *
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l24" id="l24">    24</a>  * Contributor(s):
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l25" id="l25">    25</a>  *
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l26" id="l26">    26</a>  * Alternatively, the contents of this file may be used under the terms of
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l27" id="l27">    27</a>  * either the GNU General Public License Version 2 or later (the &quot;GPL&quot;), or
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l28" id="l28">    28</a>  * the GNU Lesser General Public License Version 2.1 or later (the &quot;LGPL&quot;),
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l29" id="l29">    29</a>  * in which case the provisions of the GPL or the LGPL are applicable instead
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l30" id="l30">    30</a>  * of those above. If you wish to allow use of your version of this file only
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l31" id="l31">    31</a>  * under the terms of either the GPL or the LGPL, and not to allow others to
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l32" id="l32">    32</a>  * use your version of this file under the terms of the MPL, indicate your
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l33" id="l33">    33</a>  * decision by deleting the provisions above and replace them with the notice
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l34" id="l34">    34</a>  * and other provisions required by the GPL or the LGPL. If you do not delete
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l35" id="l35">    35</a>  * the provisions above, a recipient may use your version of this file under
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l36" id="l36">    36</a>  * the terms of any one of the MPL, the GPL or the LGPL.
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l37" id="l37">    37</a>  *
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l38" id="l38">    38</a>  * ***** END LICENSE BLOCK ***** */
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l39" id="l39">    39</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l40" id="l40">    40</a> /*
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l41" id="l41">    41</a>  * Copyright 1992,1993 Simmule Turner and Rich Salz.  All rights reserved.
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l42" id="l42">    42</a>  *
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l43" id="l43">    43</a>  * This software is not subject to any license of the American Telephone
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l44" id="l44">    44</a>  * and Telegraph Company or of the Regents of the University of California.
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l45" id="l45">    45</a>  *
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l46" id="l46">    46</a>  * Permission is granted to anyone to use this software for any purpose on
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l47" id="l47">    47</a>  * any computer system, and to alter it and redistribute it freely, subject
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l48" id="l48">    48</a>  * to the following restrictions:
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l49" id="l49">    49</a>  * 1. The authors are not responsible for the consequences of use of this
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l50" id="l50">    50</a>  *    software, no matter how awful, even if they arise from flaws in it.
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l51" id="l51">    51</a>  * 2. The origin of this software must not be misrepresented, either by
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l52" id="l52">    52</a>  *    explicit claim or by omission.  Since few users ever read sources,
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l53" id="l53">    53</a>  *    credits must appear in the documentation.
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l54" id="l54">    54</a>  * 3. Altered versions must be plainly marked as such, and must not be
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l55" id="l55">    55</a>  *    misrepresented as being the original software.  Since few users
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l56" id="l56">    56</a>  *    ever read sources, credits must appear in the documentation.
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l57" id="l57">    57</a>  * 4. This notice may not be removed or altered.
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l58" id="l58">    58</a>  */
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l59" id="l59">    59</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l60" id="l60">    60</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l61" id="l61">    61</a> /*
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l62" id="l62">    62</a> **  Main editing routines for editline library.
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l63" id="l63">    63</a> */
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l64" id="l64">    64</a> #include &quot;editline.h&quot;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l65" id="l65">    65</a> #include &lt;signal.h&gt;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l66" id="l66">    66</a> #include &lt;ctype.h&gt;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l67" id="l67">    67</a> #include &lt;unistd.h&gt;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l68" id="l68">    68</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l69" id="l69">    69</a> /*
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l70" id="l70">    70</a> **  Manifest constants.
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l71" id="l71">    71</a> */
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l72" id="l72">    72</a> #define SCREEN_WIDTH	80
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l73" id="l73">    73</a> #define SCREEN_ROWS	24
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l74" id="l74">    74</a> #define NO_ARG		(-1)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l75" id="l75">    75</a> #define DEL		127
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l76" id="l76">    76</a> #define CTL(x)		((x) &amp; 0x1F)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l77" id="l77">    77</a> #define ISCTL(x)	((x) &amp;&amp; (x) &lt; ' ')
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l78" id="l78">    78</a> #define UNCTL(x)	((x) + 64)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l79" id="l79">    79</a> #define META(x)		((x) | 0x80)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l80" id="l80">    80</a> #define ISMETA(x)	((x) &amp; 0x80)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l81" id="l81">    81</a> #define UNMETA(x)	((x) &amp; 0x7F)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l82" id="l82">    82</a> #if	!defined(HIST_SIZE)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l83" id="l83">    83</a> #define HIST_SIZE	20
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l84" id="l84">    84</a> #endif	/* !defined(HIST_SIZE) */
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l85" id="l85">    85</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l86" id="l86">    86</a> /*
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l87" id="l87">    87</a> **  Command status codes.
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l88" id="l88">    88</a> */
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l89" id="l89">    89</a> typedef enum _STATUS {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l90" id="l90">    90</a>     CSdone, CSeof, CSmove, CSdispatch, CSstay, CSsignal
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l91" id="l91">    91</a> } STATUS;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l92" id="l92">    92</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l93" id="l93">    93</a> /*
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l94" id="l94">    94</a> **  The type of case-changing to perform.
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l95" id="l95">    95</a> */
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l96" id="l96">    96</a> typedef enum _CASE {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l97" id="l97">    97</a>     TOupper, TOlower
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l98" id="l98">    98</a> } CASE;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l99" id="l99">    99</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l100" id="l100">   100</a> /*
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l101" id="l101">   101</a> **  Key to command mapping.
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l102" id="l102">   102</a> */
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l103" id="l103">   103</a> typedef struct _KEYMAP {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l104" id="l104">   104</a>     CHAR	Key;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l105" id="l105">   105</a>     STATUS	(*Function)();
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l106" id="l106">   106</a> } KEYMAP;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l107" id="l107">   107</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l108" id="l108">   108</a> /*
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l109" id="l109">   109</a> **  Command history structure.
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l110" id="l110">   110</a> */
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l111" id="l111">   111</a> typedef struct _HISTORY {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l112" id="l112">   112</a>     int		Size;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l113" id="l113">   113</a>     int		Pos;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l114" id="l114">   114</a>     CHAR	*Lines[HIST_SIZE];
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l115" id="l115">   115</a> } HISTORY;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l116" id="l116">   116</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l117" id="l117">   117</a> /*
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l118" id="l118">   118</a> **  Globals.
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l119" id="l119">   119</a> */
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l120" id="l120">   120</a> int		rl_eof;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l121" id="l121">   121</a> int		rl_erase;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l122" id="l122">   122</a> int		rl_intr;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l123" id="l123">   123</a> int		rl_kill;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l124" id="l124">   124</a> int		rl_quit;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l125" id="l125">   125</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l126" id="l126">   126</a> STATIC CHAR		NIL[] = &quot;&quot;;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l127" id="l127">   127</a> STATIC CONST CHAR	*Input = NIL;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l128" id="l128">   128</a> STATIC CHAR		*Line;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l129" id="l129">   129</a> STATIC CONST char	*Prompt;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l130" id="l130">   130</a> STATIC CHAR		*Yanked;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l131" id="l131">   131</a> STATIC char		*Screen;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l132" id="l132">   132</a> STATIC char		NEWLINE[]= CRLF;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l133" id="l133">   133</a> STATIC HISTORY		H;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l134" id="l134">   134</a> STATIC int		Repeat;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l135" id="l135">   135</a> STATIC int		End;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l136" id="l136">   136</a> STATIC int		Mark;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l137" id="l137">   137</a> STATIC int		OldPoint;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l138" id="l138">   138</a> STATIC int		Point;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l139" id="l139">   139</a> STATIC int		PushBack;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l140" id="l140">   140</a> STATIC int		Pushed;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l141" id="l141">   141</a> STATIC int		Signal;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l142" id="l142">   142</a> FORWARD KEYMAP		Map[32];
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l143" id="l143">   143</a> FORWARD KEYMAP		MetaMap[16];
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l144" id="l144">   144</a> STATIC SIZE_T		Length;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l145" id="l145">   145</a> STATIC SIZE_T		ScreenCount;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l146" id="l146">   146</a> STATIC SIZE_T		ScreenSize;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l147" id="l147">   147</a> STATIC char		*backspace;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l148" id="l148">   148</a> STATIC int		TTYwidth;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l149" id="l149">   149</a> STATIC int		TTYrows;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l150" id="l150">   150</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l151" id="l151">   151</a> /* Display print 8-bit chars as `M-x' or as the actual 8-bit char? */
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l152" id="l152">   152</a> int		rl_meta_chars = 0;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l153" id="l153">   153</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l154" id="l154">   154</a> /*
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l155" id="l155">   155</a> **  Declarations.
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l156" id="l156">   156</a> */
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l157" id="l157">   157</a> STATIC CHAR	*editinput();
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l158" id="l158">   158</a> #if	defined(USE_TERMCAP)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l159" id="l159">   159</a> #include &lt;stdlib.h&gt;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l160" id="l160">   160</a> #include &lt;curses.h&gt;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l161" id="l161">   161</a> #include &lt;term.h&gt;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l162" id="l162">   162</a> #endif	/* defined(USE_TERMCAP) */
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l163" id="l163">   163</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l164" id="l164">   164</a> /*
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l165" id="l165">   165</a> **  TTY input/output functions.
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l166" id="l166">   166</a> */
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l167" id="l167">   167</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l168" id="l168">   168</a> STATIC void
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l169" id="l169">   169</a> TTYflush()
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l170" id="l170">   170</a> {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l171" id="l171">   171</a>     if (ScreenCount) {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l172" id="l172">   172</a> 	(void)write(1, Screen, ScreenCount);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l173" id="l173">   173</a> 	ScreenCount = 0;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l174" id="l174">   174</a>     }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l175" id="l175">   175</a> }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l176" id="l176">   176</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l177" id="l177">   177</a> STATIC void
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l178" id="l178">   178</a> TTYput(c)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l179" id="l179">   179</a>     CHAR	c;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l180" id="l180">   180</a> {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l181" id="l181">   181</a>     Screen[ScreenCount] = c;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l182" id="l182">   182</a>     if (++ScreenCount &gt;= ScreenSize - 1) {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l183" id="l183">   183</a> 	ScreenSize += SCREEN_INC;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l184" id="l184">   184</a> 	RENEW(Screen, char, ScreenSize);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l185" id="l185">   185</a>     }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l186" id="l186">   186</a> }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l187" id="l187">   187</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l188" id="l188">   188</a> STATIC void
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l189" id="l189">   189</a> TTYputs(p)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l190" id="l190">   190</a>     CHAR	*p;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l191" id="l191">   191</a> {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l192" id="l192">   192</a>     while (*p)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l193" id="l193">   193</a> 	TTYput(*p++);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l194" id="l194">   194</a> }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l195" id="l195">   195</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l196" id="l196">   196</a> STATIC void
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l197" id="l197">   197</a> TTYshow(c)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l198" id="l198">   198</a>     CHAR	c;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l199" id="l199">   199</a> {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l200" id="l200">   200</a>     if (c == DEL) {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l201" id="l201">   201</a> 	TTYput('^');
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l202" id="l202">   202</a> 	TTYput('?');
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l203" id="l203">   203</a>     }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l204" id="l204">   204</a>     else if (ISCTL(c)) {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l205" id="l205">   205</a> 	TTYput('^');
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l206" id="l206">   206</a> 	TTYput(UNCTL(c));
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l207" id="l207">   207</a>     }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l208" id="l208">   208</a>     else if (rl_meta_chars &amp;&amp; ISMETA(c)) {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l209" id="l209">   209</a> 	TTYput('M');
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l210" id="l210">   210</a> 	TTYput('-');
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l211" id="l211">   211</a> 	TTYput(UNMETA(c));
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l212" id="l212">   212</a>     }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l213" id="l213">   213</a>     else
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l214" id="l214">   214</a> 	TTYput(c);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l215" id="l215">   215</a> }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l216" id="l216">   216</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l217" id="l217">   217</a> STATIC void
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l218" id="l218">   218</a> TTYstring(p)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l219" id="l219">   219</a>     CHAR	*p;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l220" id="l220">   220</a> {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l221" id="l221">   221</a>     while (*p)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l222" id="l222">   222</a> 	TTYshow(*p++);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l223" id="l223">   223</a> }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l224" id="l224">   224</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l225" id="l225">   225</a> STATIC unsigned int
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l226" id="l226">   226</a> TTYget()
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l227" id="l227">   227</a> {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l228" id="l228">   228</a>     CHAR	c;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l229" id="l229">   229</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l230" id="l230">   230</a>     TTYflush();
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l231" id="l231">   231</a>     if (Pushed) {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l232" id="l232">   232</a> 	Pushed = 0;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l233" id="l233">   233</a> 	return PushBack;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l234" id="l234">   234</a>     }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l235" id="l235">   235</a>     if (*Input)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l236" id="l236">   236</a> 	return *Input++;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l237" id="l237">   237</a>     return read(0, &amp;c, (SIZE_T)1) == 1 ? c : EOF;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l238" id="l238">   238</a> }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l239" id="l239">   239</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l240" id="l240">   240</a> #define TTYback()	(backspace ? TTYputs((CHAR *)backspace) : TTYput('\b'))
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l241" id="l241">   241</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l242" id="l242">   242</a> STATIC void
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l243" id="l243">   243</a> TTYbackn(n)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l244" id="l244">   244</a>     int		n;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l245" id="l245">   245</a> {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l246" id="l246">   246</a>     while (--n &gt;= 0)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l247" id="l247">   247</a> 	TTYback();
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l248" id="l248">   248</a> }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l249" id="l249">   249</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l250" id="l250">   250</a> STATIC void
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l251" id="l251">   251</a> TTYinfo()
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l252" id="l252">   252</a> {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l253" id="l253">   253</a>     static int		init;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l254" id="l254">   254</a> #if	defined(USE_TERMCAP)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l255" id="l255">   255</a>     char		*term;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l256" id="l256">   256</a>     char		buff[2048];
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l257" id="l257">   257</a>     char		*bp, *p;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l258" id="l258">   258</a> #endif	/* defined(USE_TERMCAP) */
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l259" id="l259">   259</a> #if	defined(TIOCGWINSZ)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l260" id="l260">   260</a>     struct winsize	W;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l261" id="l261">   261</a> #endif	/* defined(TIOCGWINSZ) */
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l262" id="l262">   262</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l263" id="l263">   263</a>     if (init) {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l264" id="l264">   264</a> #if	defined(TIOCGWINSZ)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l265" id="l265">   265</a> 	/* Perhaps we got resized. */
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l266" id="l266">   266</a> 	if (ioctl(0, TIOCGWINSZ, &amp;W) &gt;= 0
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l267" id="l267">   267</a> 	 &amp;&amp; W.ws_col &gt; 0 &amp;&amp; W.ws_row &gt; 0) {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l268" id="l268">   268</a> 	    TTYwidth = (int)W.ws_col;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l269" id="l269">   269</a> 	    TTYrows = (int)W.ws_row;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l270" id="l270">   270</a> 	}
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l271" id="l271">   271</a> #endif	/* defined(TIOCGWINSZ) */
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l272" id="l272">   272</a> 	return;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l273" id="l273">   273</a>     }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l274" id="l274">   274</a>     init++;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l275" id="l275">   275</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l276" id="l276">   276</a>     TTYwidth = TTYrows = 0;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l277" id="l277">   277</a> #if	defined(USE_TERMCAP)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l278" id="l278">   278</a>     bp = &amp;buff[0];
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l279" id="l279">   279</a>     if ((term = getenv(&quot;TERM&quot;)) == NULL)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l280" id="l280">   280</a> 	term = &quot;dumb&quot;;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l281" id="l281">   281</a>     if (tgetent(buff, term) &lt; 0) {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l282" id="l282">   282</a>        TTYwidth = SCREEN_WIDTH;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l283" id="l283">   283</a>        TTYrows = SCREEN_ROWS;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l284" id="l284">   284</a>        return;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l285" id="l285">   285</a>     }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l286" id="l286">   286</a>     p = tgetstr(&quot;le&quot;, &amp;bp);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l287" id="l287">   287</a>     backspace = p ? strdup(p) : NULL;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l288" id="l288">   288</a>     TTYwidth = tgetnum(&quot;co&quot;);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l289" id="l289">   289</a>     TTYrows = tgetnum(&quot;li&quot;);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l290" id="l290">   290</a> #endif	/* defined(USE_TERMCAP) */
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l291" id="l291">   291</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l292" id="l292">   292</a> #if	defined(TIOCGWINSZ)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l293" id="l293">   293</a>     if (ioctl(0, TIOCGWINSZ, &amp;W) &gt;= 0) {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l294" id="l294">   294</a> 	TTYwidth = (int)W.ws_col;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l295" id="l295">   295</a> 	TTYrows = (int)W.ws_row;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l296" id="l296">   296</a>     }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l297" id="l297">   297</a> #endif	/* defined(TIOCGWINSZ) */
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l298" id="l298">   298</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l299" id="l299">   299</a>     if (TTYwidth &lt;= 0 || TTYrows &lt;= 0) {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l300" id="l300">   300</a> 	TTYwidth = SCREEN_WIDTH;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l301" id="l301">   301</a> 	TTYrows = SCREEN_ROWS;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l302" id="l302">   302</a>     }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l303" id="l303">   303</a> }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l304" id="l304">   304</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l305" id="l305">   305</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l306" id="l306">   306</a> STATIC void
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l307" id="l307">   307</a> reposition()
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l308" id="l308">   308</a> {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l309" id="l309">   309</a>     int		i;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l310" id="l310">   310</a>     CHAR	*p;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l311" id="l311">   311</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l312" id="l312">   312</a>     TTYput('\r');
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l313" id="l313">   313</a>     TTYputs((CONST CHAR *)Prompt);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l314" id="l314">   314</a>     for (i = Point, p = Line; --i &gt;= 0; p++)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l315" id="l315">   315</a> 	TTYshow(*p);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l316" id="l316">   316</a> }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l317" id="l317">   317</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l318" id="l318">   318</a> STATIC void
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l319" id="l319">   319</a> left(Change)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l320" id="l320">   320</a>     STATUS	Change;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l321" id="l321">   321</a> {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l322" id="l322">   322</a>     TTYback();
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l323" id="l323">   323</a>     if (Point) {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l324" id="l324">   324</a> 	if (ISCTL(Line[Point - 1]))
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l325" id="l325">   325</a> 	    TTYback();
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l326" id="l326">   326</a>         else if (rl_meta_chars &amp;&amp; ISMETA(Line[Point - 1])) {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l327" id="l327">   327</a> 	    TTYback();
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l328" id="l328">   328</a> 	    TTYback();
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l329" id="l329">   329</a> 	}
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l330" id="l330">   330</a>     }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l331" id="l331">   331</a>     if (Change == CSmove)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l332" id="l332">   332</a> 	Point--;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l333" id="l333">   333</a> }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l334" id="l334">   334</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l335" id="l335">   335</a> STATIC void
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l336" id="l336">   336</a> right(Change)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l337" id="l337">   337</a>     STATUS	Change;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l338" id="l338">   338</a> {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l339" id="l339">   339</a>     TTYshow(Line[Point]);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l340" id="l340">   340</a>     if (Change == CSmove)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l341" id="l341">   341</a> 	Point++;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l342" id="l342">   342</a> }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l343" id="l343">   343</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l344" id="l344">   344</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l345" id="l345">   345</a> ring_bell()
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l346" id="l346">   346</a> {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l347" id="l347">   347</a>     TTYput('\07');
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l348" id="l348">   348</a>     TTYflush();
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l349" id="l349">   349</a>     return CSstay;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l350" id="l350">   350</a> }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l351" id="l351">   351</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l352" id="l352">   352</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l353" id="l353">   353</a> do_macro(c)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l354" id="l354">   354</a>     unsigned int	c;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l355" id="l355">   355</a> {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l356" id="l356">   356</a>     CHAR		name[4];
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l357" id="l357">   357</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l358" id="l358">   358</a>     name[0] = '_';
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l359" id="l359">   359</a>     name[1] = c;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l360" id="l360">   360</a>     name[2] = '_';
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l361" id="l361">   361</a>     name[3] = '\0';
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l362" id="l362">   362</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l363" id="l363">   363</a>     if ((Input = (CHAR *)getenv((char *)name)) == NULL) {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l364" id="l364">   364</a> 	Input = NIL;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l365" id="l365">   365</a> 	return ring_bell();
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l366" id="l366">   366</a>     }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l367" id="l367">   367</a>     return CSstay;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l368" id="l368">   368</a> }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l369" id="l369">   369</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l370" id="l370">   370</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l371" id="l371">   371</a> do_forward(move)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l372" id="l372">   372</a>     STATUS	move;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l373" id="l373">   373</a> {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l374" id="l374">   374</a>     int		i;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l375" id="l375">   375</a>     CHAR	*p;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l376" id="l376">   376</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l377" id="l377">   377</a>     i = 0;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l378" id="l378">   378</a>     do {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l379" id="l379">   379</a> 	p = &amp;Line[Point];
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l380" id="l380">   380</a> 	for ( ; Point &lt; End &amp;&amp; (*p == ' ' || !isalnum(*p)); Point++, p++)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l381" id="l381">   381</a> 	    if (move == CSmove)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l382" id="l382">   382</a> 		right(CSstay);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l383" id="l383">   383</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l384" id="l384">   384</a> 	for (; Point &lt; End &amp;&amp; isalnum(*p); Point++, p++)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l385" id="l385">   385</a> 	    if (move == CSmove)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l386" id="l386">   386</a> 		right(CSstay);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l387" id="l387">   387</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l388" id="l388">   388</a> 	if (Point == End)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l389" id="l389">   389</a> 	    break;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l390" id="l390">   390</a>     } while (++i &lt; Repeat);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l391" id="l391">   391</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l392" id="l392">   392</a>     return CSstay;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l393" id="l393">   393</a> }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l394" id="l394">   394</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l395" id="l395">   395</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l396" id="l396">   396</a> do_case(type)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l397" id="l397">   397</a>     CASE	type;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l398" id="l398">   398</a> {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l399" id="l399">   399</a>     int		i;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l400" id="l400">   400</a>     int		end;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l401" id="l401">   401</a>     int		count;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l402" id="l402">   402</a>     CHAR	*p;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l403" id="l403">   403</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l404" id="l404">   404</a>     (void)do_forward(CSstay);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l405" id="l405">   405</a>     if (OldPoint != Point) {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l406" id="l406">   406</a> 	if ((count = Point - OldPoint) &lt; 0)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l407" id="l407">   407</a> 	    count = -count;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l408" id="l408">   408</a> 	Point = OldPoint;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l409" id="l409">   409</a> 	if ((end = Point + count) &gt; End)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l410" id="l410">   410</a> 	    end = End;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l411" id="l411">   411</a> 	for (i = Point, p = &amp;Line[i]; i &lt; end; i++, p++) {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l412" id="l412">   412</a> 	    if (type == TOupper) {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l413" id="l413">   413</a> 		if (islower(*p))
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l414" id="l414">   414</a> 		    *p = toupper(*p);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l415" id="l415">   415</a> 	    }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l416" id="l416">   416</a> 	    else if (isupper(*p))
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l417" id="l417">   417</a> 		*p = tolower(*p);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l418" id="l418">   418</a> 	    right(CSmove);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l419" id="l419">   419</a> 	}
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l420" id="l420">   420</a>     }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l421" id="l421">   421</a>     return CSstay;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l422" id="l422">   422</a> }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l423" id="l423">   423</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l424" id="l424">   424</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l425" id="l425">   425</a> case_down_word()
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l426" id="l426">   426</a> {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l427" id="l427">   427</a>     return do_case(TOlower);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l428" id="l428">   428</a> }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l429" id="l429">   429</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l430" id="l430">   430</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l431" id="l431">   431</a> case_up_word()
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l432" id="l432">   432</a> {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l433" id="l433">   433</a>     return do_case(TOupper);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l434" id="l434">   434</a> }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l435" id="l435">   435</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l436" id="l436">   436</a> STATIC void
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l437" id="l437">   437</a> ceol()
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l438" id="l438">   438</a> {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l439" id="l439">   439</a>     int		extras;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l440" id="l440">   440</a>     int		i;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l441" id="l441">   441</a>     CHAR	*p;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l442" id="l442">   442</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l443" id="l443">   443</a>     for (extras = 0, i = Point, p = &amp;Line[i]; i &lt;= End; i++, p++) {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l444" id="l444">   444</a> 	TTYput(' ');
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l445" id="l445">   445</a> 	if (ISCTL(*p)) {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l446" id="l446">   446</a> 	    TTYput(' ');
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l447" id="l447">   447</a> 	    extras++;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l448" id="l448">   448</a> 	}
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l449" id="l449">   449</a> 	else if (rl_meta_chars &amp;&amp; ISMETA(*p)) {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l450" id="l450">   450</a> 	    TTYput(' ');
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l451" id="l451">   451</a> 	    TTYput(' ');
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l452" id="l452">   452</a> 	    extras += 2;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l453" id="l453">   453</a> 	}
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l454" id="l454">   454</a>     }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l455" id="l455">   455</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l456" id="l456">   456</a>     for (i += extras; i &gt; Point; i--)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l457" id="l457">   457</a> 	TTYback();
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l458" id="l458">   458</a> }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l459" id="l459">   459</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l460" id="l460">   460</a> STATIC void
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l461" id="l461">   461</a> clear_line()
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l462" id="l462">   462</a> {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l463" id="l463">   463</a>     Point = -strlen(Prompt);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l464" id="l464">   464</a>     TTYput('\r');
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l465" id="l465">   465</a>     ceol();
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l466" id="l466">   466</a>     Point = 0;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l467" id="l467">   467</a>     End = 0;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l468" id="l468">   468</a>     Line[0] = '\0';
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l469" id="l469">   469</a> }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l470" id="l470">   470</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l471" id="l471">   471</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l472" id="l472">   472</a> insert_string(p)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l473" id="l473">   473</a>     CHAR	*p;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l474" id="l474">   474</a> {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l475" id="l475">   475</a>     SIZE_T	len;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l476" id="l476">   476</a>     int		i;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l477" id="l477">   477</a>     CHAR	*new;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l478" id="l478">   478</a>     CHAR	*q;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l479" id="l479">   479</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l480" id="l480">   480</a>     len = strlen((char *)p);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l481" id="l481">   481</a>     if (End + len &gt;= Length) {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l482" id="l482">   482</a> 	if ((new = NEW(CHAR, Length + len + MEM_INC)) == NULL)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l483" id="l483">   483</a> 	    return CSstay;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l484" id="l484">   484</a> 	if (Length) {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l485" id="l485">   485</a> 	    COPYFROMTO(new, Line, Length);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l486" id="l486">   486</a> 	    DISPOSE(Line);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l487" id="l487">   487</a> 	}
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l488" id="l488">   488</a> 	Line = new;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l489" id="l489">   489</a> 	Length += len + MEM_INC;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l490" id="l490">   490</a>     }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l491" id="l491">   491</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l492" id="l492">   492</a>     for (q = &amp;Line[Point], i = End - Point; --i &gt;= 0; )
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l493" id="l493">   493</a> 	q[len + i] = q[i];
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l494" id="l494">   494</a>     COPYFROMTO(&amp;Line[Point], p, len);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l495" id="l495">   495</a>     End += len;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l496" id="l496">   496</a>     Line[End] = '\0';
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l497" id="l497">   497</a>     TTYstring(&amp;Line[Point]);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l498" id="l498">   498</a>     Point += len;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l499" id="l499">   499</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l500" id="l500">   500</a>     return Point == End ? CSstay : CSmove;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l501" id="l501">   501</a> }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l502" id="l502">   502</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l503" id="l503">   503</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l504" id="l504">   504</a> redisplay()
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l505" id="l505">   505</a> {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l506" id="l506">   506</a>     TTYputs((CONST CHAR *)NEWLINE);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l507" id="l507">   507</a>     TTYputs((CONST CHAR *)Prompt);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l508" id="l508">   508</a>     TTYstring(Line);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l509" id="l509">   509</a>     return CSmove;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l510" id="l510">   510</a> }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l511" id="l511">   511</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l512" id="l512">   512</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l513" id="l513">   513</a> toggle_meta_mode()
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l514" id="l514">   514</a> {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l515" id="l515">   515</a>     rl_meta_chars = ! rl_meta_chars;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l516" id="l516">   516</a>     return redisplay();
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l517" id="l517">   517</a> }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l518" id="l518">   518</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l519" id="l519">   519</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l520" id="l520">   520</a> STATIC CHAR *
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l521" id="l521">   521</a> next_hist()
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l522" id="l522">   522</a> {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l523" id="l523">   523</a>     return H.Pos &gt;= H.Size - 1 ? NULL : H.Lines[++H.Pos];
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l524" id="l524">   524</a> }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l525" id="l525">   525</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l526" id="l526">   526</a> STATIC CHAR *
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l527" id="l527">   527</a> prev_hist()
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l528" id="l528">   528</a> {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l529" id="l529">   529</a>     return H.Pos == 0 ? NULL : H.Lines[--H.Pos];
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l530" id="l530">   530</a> }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l531" id="l531">   531</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l532" id="l532">   532</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l533" id="l533">   533</a> do_insert_hist(p)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l534" id="l534">   534</a>     CHAR	*p;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l535" id="l535">   535</a> {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l536" id="l536">   536</a>     if (p == NULL)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l537" id="l537">   537</a> 	return ring_bell();
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l538" id="l538">   538</a>     Point = 0;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l539" id="l539">   539</a>     reposition();
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l540" id="l540">   540</a>     ceol();
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l541" id="l541">   541</a>     End = 0;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l542" id="l542">   542</a>     return insert_string(p);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l543" id="l543">   543</a> }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l544" id="l544">   544</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l545" id="l545">   545</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l546" id="l546">   546</a> do_hist(move)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l547" id="l547">   547</a>     CHAR	*(*move)();
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l548" id="l548">   548</a> {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l549" id="l549">   549</a>     CHAR	*p;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l550" id="l550">   550</a>     int		i;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l551" id="l551">   551</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l552" id="l552">   552</a>     i = 0;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l553" id="l553">   553</a>     do {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l554" id="l554">   554</a> 	if ((p = (*move)()) == NULL)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l555" id="l555">   555</a> 	    return ring_bell();
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l556" id="l556">   556</a>     } while (++i &lt; Repeat);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l557" id="l557">   557</a>     return do_insert_hist(p);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l558" id="l558">   558</a> }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l559" id="l559">   559</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l560" id="l560">   560</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l561" id="l561">   561</a> h_next()
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l562" id="l562">   562</a> {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l563" id="l563">   563</a>     return do_hist(next_hist);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l564" id="l564">   564</a> }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l565" id="l565">   565</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l566" id="l566">   566</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l567" id="l567">   567</a> h_prev()
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l568" id="l568">   568</a> {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l569" id="l569">   569</a>     return do_hist(prev_hist);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l570" id="l570">   570</a> }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l571" id="l571">   571</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l572" id="l572">   572</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l573" id="l573">   573</a> h_first()
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l574" id="l574">   574</a> {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l575" id="l575">   575</a>     return do_insert_hist(H.Lines[H.Pos = 0]);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l576" id="l576">   576</a> }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l577" id="l577">   577</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l578" id="l578">   578</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l579" id="l579">   579</a> h_last()
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l580" id="l580">   580</a> {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l581" id="l581">   581</a>     return do_insert_hist(H.Lines[H.Pos = H.Size - 1]);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l582" id="l582">   582</a> }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l583" id="l583">   583</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l584" id="l584">   584</a> /*
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l585" id="l585">   585</a> **  Return zero if pat appears as a substring in text.
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l586" id="l586">   586</a> */
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l587" id="l587">   587</a> STATIC int
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l588" id="l588">   588</a> substrcmp(text, pat, len)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l589" id="l589">   589</a>     char	*text;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l590" id="l590">   590</a>     char	*pat;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l591" id="l591">   591</a>     int		len;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l592" id="l592">   592</a> {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l593" id="l593">   593</a>     char	c;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l594" id="l594">   594</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l595" id="l595">   595</a>     if ((c = *pat) == '\0')
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l596" id="l596">   596</a>         return *text == '\0';
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l597" id="l597">   597</a>     for ( ; *text; text++)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l598" id="l598">   598</a>         if (*text == c &amp;&amp; strncmp(text, pat, len) == 0)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l599" id="l599">   599</a>             return 0;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l600" id="l600">   600</a>     return 1;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l601" id="l601">   601</a> }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l602" id="l602">   602</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l603" id="l603">   603</a> STATIC CHAR *
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l604" id="l604">   604</a> search_hist(search, move)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l605" id="l605">   605</a>     CHAR	*search;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l606" id="l606">   606</a>     CHAR	*(*move)();
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l607" id="l607">   607</a> {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l608" id="l608">   608</a>     static CHAR	*old_search;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l609" id="l609">   609</a>     int		len;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l610" id="l610">   610</a>     int		pos;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l611" id="l611">   611</a>     int		(*match)();
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l612" id="l612">   612</a>     char	*pat;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l613" id="l613">   613</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l614" id="l614">   614</a>     /* Save or get remembered search pattern. */
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l615" id="l615">   615</a>     if (search &amp;&amp; *search) {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l616" id="l616">   616</a> 	if (old_search)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l617" id="l617">   617</a> 	    DISPOSE(old_search);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l618" id="l618">   618</a> 	old_search = (CHAR *)strdup((char *)search);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l619" id="l619">   619</a>     }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l620" id="l620">   620</a>     else {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l621" id="l621">   621</a> 	if (old_search == NULL || *old_search == '\0')
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l622" id="l622">   622</a>             return NULL;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l623" id="l623">   623</a> 	search = old_search;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l624" id="l624">   624</a>     }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l625" id="l625">   625</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l626" id="l626">   626</a>     /* Set up pattern-finder. */
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l627" id="l627">   627</a>     if (*search == '^') {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l628" id="l628">   628</a> 	match = strncmp;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l629" id="l629">   629</a> 	pat = (char *)(search + 1);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l630" id="l630">   630</a>     }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l631" id="l631">   631</a>     else {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l632" id="l632">   632</a> 	match = substrcmp;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l633" id="l633">   633</a> 	pat = (char *)search;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l634" id="l634">   634</a>     }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l635" id="l635">   635</a>     len = strlen(pat);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l636" id="l636">   636</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l637" id="l637">   637</a>     for (pos = H.Pos; (*move)() != NULL; )
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l638" id="l638">   638</a> 	if ((*match)((char *)H.Lines[H.Pos], pat, len) == 0)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l639" id="l639">   639</a>             return H.Lines[H.Pos];
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l640" id="l640">   640</a>     H.Pos = pos;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l641" id="l641">   641</a>     return NULL;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l642" id="l642">   642</a> }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l643" id="l643">   643</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l644" id="l644">   644</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l645" id="l645">   645</a> h_search()
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l646" id="l646">   646</a> {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l647" id="l647">   647</a>     static int	Searching;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l648" id="l648">   648</a>     CONST char	*old_prompt;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l649" id="l649">   649</a>     CHAR	*(*move)();
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l650" id="l650">   650</a>     CHAR	*p;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l651" id="l651">   651</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l652" id="l652">   652</a>     if (Searching)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l653" id="l653">   653</a> 	return ring_bell();
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l654" id="l654">   654</a>     Searching = 1;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l655" id="l655">   655</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l656" id="l656">   656</a>     clear_line();
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l657" id="l657">   657</a>     old_prompt = Prompt;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l658" id="l658">   658</a>     Prompt = &quot;Search: &quot;;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l659" id="l659">   659</a>     TTYputs((CONST CHAR *)Prompt);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l660" id="l660">   660</a>     move = Repeat == NO_ARG ? prev_hist : next_hist;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l661" id="l661">   661</a>     p = editinput();
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l662" id="l662">   662</a>     Prompt = old_prompt;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l663" id="l663">   663</a>     Searching = 0;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l664" id="l664">   664</a>     TTYputs((CONST CHAR *)Prompt);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l665" id="l665">   665</a>     if (p == NULL &amp;&amp; Signal &gt; 0) {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l666" id="l666">   666</a> 	Signal = 0;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l667" id="l667">   667</a> 	clear_line();
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l668" id="l668">   668</a> 	return redisplay();
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l669" id="l669">   669</a>     }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l670" id="l670">   670</a>     p = search_hist(p, move);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l671" id="l671">   671</a>     clear_line();
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l672" id="l672">   672</a>     if (p == NULL) {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l673" id="l673">   673</a> 	(void)ring_bell();
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l674" id="l674">   674</a> 	return redisplay();
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l675" id="l675">   675</a>     }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l676" id="l676">   676</a>     return do_insert_hist(p);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l677" id="l677">   677</a> }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l678" id="l678">   678</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l679" id="l679">   679</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l680" id="l680">   680</a> fd_char()
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l681" id="l681">   681</a> {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l682" id="l682">   682</a>     int		i;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l683" id="l683">   683</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l684" id="l684">   684</a>     i = 0;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l685" id="l685">   685</a>     do {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l686" id="l686">   686</a> 	if (Point &gt;= End)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l687" id="l687">   687</a> 	    break;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l688" id="l688">   688</a> 	right(CSmove);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l689" id="l689">   689</a>     } while (++i &lt; Repeat);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l690" id="l690">   690</a>     return CSstay;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l691" id="l691">   691</a> }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l692" id="l692">   692</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l693" id="l693">   693</a> STATIC void
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l694" id="l694">   694</a> save_yank(begin, i)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l695" id="l695">   695</a>     int		begin;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l696" id="l696">   696</a>     int		i;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l697" id="l697">   697</a> {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l698" id="l698">   698</a>     if (Yanked) {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l699" id="l699">   699</a> 	DISPOSE(Yanked);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l700" id="l700">   700</a> 	Yanked = NULL;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l701" id="l701">   701</a>     }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l702" id="l702">   702</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l703" id="l703">   703</a>     if (i &lt; 1)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l704" id="l704">   704</a> 	return;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l705" id="l705">   705</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l706" id="l706">   706</a>     if ((Yanked = NEW(CHAR, (SIZE_T)i + 1)) != NULL) {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l707" id="l707">   707</a> 	COPYFROMTO(Yanked, &amp;Line[begin], i);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l708" id="l708">   708</a> 	Yanked[i] = '\0';
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l709" id="l709">   709</a>     }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l710" id="l710">   710</a> }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l711" id="l711">   711</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l712" id="l712">   712</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l713" id="l713">   713</a> delete_string(count)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l714" id="l714">   714</a>     int		count;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l715" id="l715">   715</a> {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l716" id="l716">   716</a>     int		i;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l717" id="l717">   717</a>     CHAR	*p;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l718" id="l718">   718</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l719" id="l719">   719</a>     if (count &lt;= 0 || End == Point)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l720" id="l720">   720</a> 	return ring_bell();
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l721" id="l721">   721</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l722" id="l722">   722</a>     if (count == 1 &amp;&amp; Point == End - 1) {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l723" id="l723">   723</a> 	/* Optimize common case of delete at end of line. */
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l724" id="l724">   724</a> 	End--;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l725" id="l725">   725</a> 	p = &amp;Line[Point];
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l726" id="l726">   726</a> 	i = 1;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l727" id="l727">   727</a> 	TTYput(' ');
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l728" id="l728">   728</a> 	if (ISCTL(*p)) {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l729" id="l729">   729</a> 	    i = 2;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l730" id="l730">   730</a> 	    TTYput(' ');
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l731" id="l731">   731</a> 	}
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l732" id="l732">   732</a> 	else if (rl_meta_chars &amp;&amp; ISMETA(*p)) {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l733" id="l733">   733</a> 	    i = 3;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l734" id="l734">   734</a> 	    TTYput(' ');
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l735" id="l735">   735</a> 	    TTYput(' ');
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l736" id="l736">   736</a> 	}
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l737" id="l737">   737</a> 	TTYbackn(i);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l738" id="l738">   738</a> 	*p = '\0';
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l739" id="l739">   739</a> 	return CSmove;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l740" id="l740">   740</a>     }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l741" id="l741">   741</a>     if (Point + count &gt; End &amp;&amp; (count = End - Point) &lt;= 0)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l742" id="l742">   742</a> 	return CSstay;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l743" id="l743">   743</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l744" id="l744">   744</a>     if (count &gt; 1)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l745" id="l745">   745</a> 	save_yank(Point, count);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l746" id="l746">   746</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l747" id="l747">   747</a>     for (p = &amp;Line[Point], i = End - (Point + count) + 1; --i &gt;= 0; p++)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l748" id="l748">   748</a> 	p[0] = p[count];
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l749" id="l749">   749</a>     ceol();
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l750" id="l750">   750</a>     End -= count;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l751" id="l751">   751</a>     TTYstring(&amp;Line[Point]);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l752" id="l752">   752</a>     return CSmove;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l753" id="l753">   753</a> }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l754" id="l754">   754</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l755" id="l755">   755</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l756" id="l756">   756</a> bk_char()
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l757" id="l757">   757</a> {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l758" id="l758">   758</a>     int		i;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l759" id="l759">   759</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l760" id="l760">   760</a>     i = 0;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l761" id="l761">   761</a>     do {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l762" id="l762">   762</a> 	if (Point == 0)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l763" id="l763">   763</a> 	    break;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l764" id="l764">   764</a> 	left(CSmove);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l765" id="l765">   765</a>     } while (++i &lt; Repeat);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l766" id="l766">   766</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l767" id="l767">   767</a>     return CSstay;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l768" id="l768">   768</a> }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l769" id="l769">   769</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l770" id="l770">   770</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l771" id="l771">   771</a> bk_del_char()
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l772" id="l772">   772</a> {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l773" id="l773">   773</a>     int		i;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l774" id="l774">   774</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l775" id="l775">   775</a>     i = 0;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l776" id="l776">   776</a>     do {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l777" id="l777">   777</a> 	if (Point == 0)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l778" id="l778">   778</a> 	    break;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l779" id="l779">   779</a> 	left(CSmove);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l780" id="l780">   780</a>     } while (++i &lt; Repeat);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l781" id="l781">   781</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l782" id="l782">   782</a>     return delete_string(i);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l783" id="l783">   783</a> }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l784" id="l784">   784</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l785" id="l785">   785</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l786" id="l786">   786</a> kill_line()
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l787" id="l787">   787</a> {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l788" id="l788">   788</a>     int		i;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l789" id="l789">   789</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l790" id="l790">   790</a>     if (Repeat != NO_ARG) {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l791" id="l791">   791</a> 	if (Repeat &lt; Point) {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l792" id="l792">   792</a> 	    i = Point;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l793" id="l793">   793</a> 	    Point = Repeat;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l794" id="l794">   794</a> 	    reposition();
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l795" id="l795">   795</a> 	    (void)delete_string(i - Point);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l796" id="l796">   796</a> 	}
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l797" id="l797">   797</a> 	else if (Repeat &gt; Point) {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l798" id="l798">   798</a> 	    right(CSmove);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l799" id="l799">   799</a> 	    (void)delete_string(Repeat - Point - 1);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l800" id="l800">   800</a> 	}
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l801" id="l801">   801</a> 	return CSmove;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l802" id="l802">   802</a>     }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l803" id="l803">   803</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l804" id="l804">   804</a>     save_yank(Point, End - Point);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l805" id="l805">   805</a>     Line[Point] = '\0';
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l806" id="l806">   806</a>     ceol();
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l807" id="l807">   807</a>     End = Point;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l808" id="l808">   808</a>     return CSstay;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l809" id="l809">   809</a> }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l810" id="l810">   810</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l811" id="l811">   811</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l812" id="l812">   812</a> insert_char(c)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l813" id="l813">   813</a>     int		c;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l814" id="l814">   814</a> {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l815" id="l815">   815</a>     STATUS	s;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l816" id="l816">   816</a>     CHAR	buff[2];
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l817" id="l817">   817</a>     CHAR	*p;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l818" id="l818">   818</a>     CHAR	*q;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l819" id="l819">   819</a>     int		i;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l820" id="l820">   820</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l821" id="l821">   821</a>     if (Repeat == NO_ARG || Repeat &lt; 2) {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l822" id="l822">   822</a> 	buff[0] = c;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l823" id="l823">   823</a> 	buff[1] = '\0';
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l824" id="l824">   824</a> 	return insert_string(buff);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l825" id="l825">   825</a>     }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l826" id="l826">   826</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l827" id="l827">   827</a>     if ((p = NEW(CHAR, Repeat + 1)) == NULL)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l828" id="l828">   828</a> 	return CSstay;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l829" id="l829">   829</a>     for (i = Repeat, q = p; --i &gt;= 0; )
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l830" id="l830">   830</a> 	*q++ = c;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l831" id="l831">   831</a>     *q = '\0';
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l832" id="l832">   832</a>     Repeat = 0;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l833" id="l833">   833</a>     s = insert_string(p);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l834" id="l834">   834</a>     DISPOSE(p);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l835" id="l835">   835</a>     return s;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l836" id="l836">   836</a> }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l837" id="l837">   837</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l838" id="l838">   838</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l839" id="l839">   839</a> meta()
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l840" id="l840">   840</a> {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l841" id="l841">   841</a>     unsigned int	c;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l842" id="l842">   842</a>     KEYMAP		*kp;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l843" id="l843">   843</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l844" id="l844">   844</a>     if ((c = TTYget()) == EOF)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l845" id="l845">   845</a> 	return CSeof;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l846" id="l846">   846</a> #if	defined(ANSI_ARROWS)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l847" id="l847">   847</a>     /* Also include VT-100 arrows. */
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l848" id="l848">   848</a>     if (c == '[' || c == 'O')
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l849" id="l849">   849</a> 	switch (c = TTYget()) {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l850" id="l850">   850</a> 	default:	return ring_bell();
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l851" id="l851">   851</a> 	case EOF:	return CSeof;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l852" id="l852">   852</a> 	case 'A':	return h_prev();
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l853" id="l853">   853</a> 	case 'B':	return h_next();
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l854" id="l854">   854</a> 	case 'C':	return fd_char();
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l855" id="l855">   855</a> 	case 'D':	return bk_char();
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l856" id="l856">   856</a> 	}
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l857" id="l857">   857</a> #endif	/* defined(ANSI_ARROWS) */
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l858" id="l858">   858</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l859" id="l859">   859</a>     if (isdigit(c)) {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l860" id="l860">   860</a> 	for (Repeat = c - '0'; (c = TTYget()) != EOF &amp;&amp; isdigit(c); )
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l861" id="l861">   861</a> 	    Repeat = Repeat * 10 + c - '0';
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l862" id="l862">   862</a> 	Pushed = 1;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l863" id="l863">   863</a> 	PushBack = c;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l864" id="l864">   864</a> 	return CSstay;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l865" id="l865">   865</a>     }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l866" id="l866">   866</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l867" id="l867">   867</a>     if (isupper(c))
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l868" id="l868">   868</a> 	return do_macro(c);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l869" id="l869">   869</a>     for (OldPoint = Point, kp = MetaMap; kp-&gt;Function; kp++)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l870" id="l870">   870</a> 	if (kp-&gt;Key == c)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l871" id="l871">   871</a> 	    return (*kp-&gt;Function)();
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l872" id="l872">   872</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l873" id="l873">   873</a>     return ring_bell();
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l874" id="l874">   874</a> }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l875" id="l875">   875</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l876" id="l876">   876</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l877" id="l877">   877</a> emacs(c)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l878" id="l878">   878</a>     unsigned int	c;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l879" id="l879">   879</a> {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l880" id="l880">   880</a>     STATUS		s;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l881" id="l881">   881</a>     KEYMAP		*kp;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l882" id="l882">   882</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l883" id="l883">   883</a>     if (rl_meta_chars &amp;&amp; ISMETA(c)) {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l884" id="l884">   884</a> 	Pushed = 1;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l885" id="l885">   885</a> 	PushBack = UNMETA(c);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l886" id="l886">   886</a> 	return meta();
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l887" id="l887">   887</a>     }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l888" id="l888">   888</a>     for (kp = Map; kp-&gt;Function; kp++)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l889" id="l889">   889</a> 	if (kp-&gt;Key == c)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l890" id="l890">   890</a> 	    break;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l891" id="l891">   891</a>     s = kp-&gt;Function ? (*kp-&gt;Function)() : insert_char((int)c);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l892" id="l892">   892</a>     if (!Pushed)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l893" id="l893">   893</a> 	/* No pushback means no repeat count; hacky, but true. */
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l894" id="l894">   894</a> 	Repeat = NO_ARG;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l895" id="l895">   895</a>     return s;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l896" id="l896">   896</a> }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l897" id="l897">   897</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l898" id="l898">   898</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l899" id="l899">   899</a> TTYspecial(c)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l900" id="l900">   900</a>     unsigned int	c;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l901" id="l901">   901</a> {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l902" id="l902">   902</a>     if (ISMETA(c))
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l903" id="l903">   903</a> 	return CSdispatch;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l904" id="l904">   904</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l905" id="l905">   905</a>     if (c == rl_erase || c == DEL)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l906" id="l906">   906</a> 	return bk_del_char();
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l907" id="l907">   907</a>     if (c == rl_kill) {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l908" id="l908">   908</a> 	if (Point != 0) {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l909" id="l909">   909</a> 	    Point = 0;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l910" id="l910">   910</a> 	    reposition();
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l911" id="l911">   911</a> 	}
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l912" id="l912">   912</a> 	Repeat = NO_ARG;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l913" id="l913">   913</a> 	return kill_line();
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l914" id="l914">   914</a>     }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l915" id="l915">   915</a>     if (c == rl_eof &amp;&amp; Point == 0 &amp;&amp; End == 0)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l916" id="l916">   916</a> 	return CSeof;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l917" id="l917">   917</a>     if (c == rl_intr) {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l918" id="l918">   918</a> 	Signal = SIGINT;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l919" id="l919">   919</a> 	return CSsignal;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l920" id="l920">   920</a>     }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l921" id="l921">   921</a>     if (c == rl_quit) {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l922" id="l922">   922</a> 	Signal = SIGQUIT;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l923" id="l923">   923</a> 	return CSeof;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l924" id="l924">   924</a>     }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l925" id="l925">   925</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l926" id="l926">   926</a>     return CSdispatch;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l927" id="l927">   927</a> }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l928" id="l928">   928</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l929" id="l929">   929</a> STATIC CHAR *
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l930" id="l930">   930</a> editinput()
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l931" id="l931">   931</a> {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l932" id="l932">   932</a>     unsigned int	c;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l933" id="l933">   933</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l934" id="l934">   934</a>     Repeat = NO_ARG;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l935" id="l935">   935</a>     OldPoint = Point = Mark = End = 0;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l936" id="l936">   936</a>     Line[0] = '\0';
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l937" id="l937">   937</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l938" id="l938">   938</a>     Signal = -1;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l939" id="l939">   939</a>     while ((c = TTYget()) != EOF)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l940" id="l940">   940</a> 	switch (TTYspecial(c)) {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l941" id="l941">   941</a> 	case CSdone:
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l942" id="l942">   942</a> 	    return Line;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l943" id="l943">   943</a> 	case CSeof:
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l944" id="l944">   944</a> 	    return NULL;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l945" id="l945">   945</a> 	case CSsignal:
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l946" id="l946">   946</a> 	    return (CHAR *)&quot;&quot;;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l947" id="l947">   947</a> 	case CSmove:
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l948" id="l948">   948</a> 	    reposition();
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l949" id="l949">   949</a> 	    break;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l950" id="l950">   950</a> 	case CSdispatch:
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l951" id="l951">   951</a> 	    switch (emacs(c)) {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l952" id="l952">   952</a> 	    case CSdone:
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l953" id="l953">   953</a> 		return Line;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l954" id="l954">   954</a> 	    case CSeof:
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l955" id="l955">   955</a> 		return NULL;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l956" id="l956">   956</a> 	    case CSsignal:
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l957" id="l957">   957</a> 		return (CHAR *)&quot;&quot;;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l958" id="l958">   958</a> 	    case CSmove:
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l959" id="l959">   959</a> 		reposition();
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l960" id="l960">   960</a> 		break;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l961" id="l961">   961</a> 	    case CSdispatch:
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l962" id="l962">   962</a> 	    case CSstay:
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l963" id="l963">   963</a> 		break;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l964" id="l964">   964</a> 	    }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l965" id="l965">   965</a> 	    break;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l966" id="l966">   966</a> 	case CSstay:
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l967" id="l967">   967</a> 	    break;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l968" id="l968">   968</a> 	}
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l969" id="l969">   969</a>     if (strlen(Line))
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l970" id="l970">   970</a>         return Line;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l971" id="l971">   971</a>     free(Line);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l972" id="l972">   972</a>     return NULL;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l973" id="l973">   973</a> }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l974" id="l974">   974</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l975" id="l975">   975</a> STATIC void
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l976" id="l976">   976</a> hist_add(p)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l977" id="l977">   977</a>     CHAR	*p;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l978" id="l978">   978</a> {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l979" id="l979">   979</a>     int		i;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l980" id="l980">   980</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l981" id="l981">   981</a>     if ((p = (CHAR *)strdup((char *)p)) == NULL)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l982" id="l982">   982</a> 	return;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l983" id="l983">   983</a>     if (H.Size &lt; HIST_SIZE)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l984" id="l984">   984</a> 	H.Lines[H.Size++] = p;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l985" id="l985">   985</a>     else {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l986" id="l986">   986</a> 	DISPOSE(H.Lines[0]);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l987" id="l987">   987</a> 	for (i = 0; i &lt; HIST_SIZE - 1; i++)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l988" id="l988">   988</a> 	    H.Lines[i] = H.Lines[i + 1];
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l989" id="l989">   989</a> 	H.Lines[i] = p;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l990" id="l990">   990</a>     }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l991" id="l991">   991</a>     H.Pos = H.Size - 1;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l992" id="l992">   992</a> }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l993" id="l993">   993</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l994" id="l994">   994</a> /*
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l995" id="l995">   995</a> **  For compatibility with FSF readline.
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l996" id="l996">   996</a> */
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l997" id="l997">   997</a> /* ARGSUSED0 */
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l998" id="l998">   998</a> void
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l999" id="l999">   999</a> rl_reset_terminal(p)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1000" id="l1000">  1000</a>     char	*p;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1001" id="l1001">  1001</a> {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1002" id="l1002">  1002</a> }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1003" id="l1003">  1003</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1004" id="l1004">  1004</a> void
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1005" id="l1005">  1005</a> rl_initialize()
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1006" id="l1006">  1006</a> {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1007" id="l1007">  1007</a> }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1008" id="l1008">  1008</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1009" id="l1009">  1009</a> char *
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1010" id="l1010">  1010</a> readline(prompt)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1011" id="l1011">  1011</a>     CONST char	*prompt;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1012" id="l1012">  1012</a> {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1013" id="l1013">  1013</a>     CHAR	*line;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1014" id="l1014">  1014</a>     int		s;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1015" id="l1015">  1015</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1016" id="l1016">  1016</a>     if (Line == NULL) {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1017" id="l1017">  1017</a> 	Length = MEM_INC;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1018" id="l1018">  1018</a> 	if ((Line = NEW(CHAR, Length)) == NULL)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1019" id="l1019">  1019</a> 	    return NULL;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1020" id="l1020">  1020</a>     }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1021" id="l1021">  1021</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1022" id="l1022">  1022</a>     TTYinfo();
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1023" id="l1023">  1023</a>     rl_ttyset(0);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1024" id="l1024">  1024</a>     hist_add(NIL);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1025" id="l1025">  1025</a>     ScreenSize = SCREEN_INC;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1026" id="l1026">  1026</a>     Screen = NEW(char, ScreenSize);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1027" id="l1027">  1027</a>     Prompt = prompt ? prompt : (char *)NIL;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1028" id="l1028">  1028</a>     TTYputs((CONST CHAR *)Prompt);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1029" id="l1029">  1029</a>     if ((line = editinput()) != NULL) {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1030" id="l1030">  1030</a> 	line = (CHAR *)strdup((char *)line);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1031" id="l1031">  1031</a> 	TTYputs((CHAR *)NEWLINE);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1032" id="l1032">  1032</a> 	TTYflush();
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1033" id="l1033">  1033</a>     }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1034" id="l1034">  1034</a>     rl_ttyset(1);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1035" id="l1035">  1035</a>     DISPOSE(Screen);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1036" id="l1036">  1036</a>     DISPOSE(H.Lines[--H.Size]);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1037" id="l1037">  1037</a>     if (Signal &gt; 0) {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1038" id="l1038">  1038</a> 	s = Signal;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1039" id="l1039">  1039</a> 	Signal = 0;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1040" id="l1040">  1040</a> 	(void)kill(getpid(), s);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1041" id="l1041">  1041</a>     }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1042" id="l1042">  1042</a>     return (char *)line;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1043" id="l1043">  1043</a> }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1044" id="l1044">  1044</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1045" id="l1045">  1045</a> void
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1046" id="l1046">  1046</a> add_history(p)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1047" id="l1047">  1047</a>     char	*p;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1048" id="l1048">  1048</a> {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1049" id="l1049">  1049</a>     if (p == NULL || *p == '\0')
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1050" id="l1050">  1050</a> 	return;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1051" id="l1051">  1051</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1052" id="l1052">  1052</a> #if	defined(UNIQUE_HISTORY)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1053" id="l1053">  1053</a>     if (H.Size &amp;&amp; strcmp(p, H.Lines[H.Size - 1]) == 0)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1054" id="l1054">  1054</a>         return;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1055" id="l1055">  1055</a> #endif	/* defined(UNIQUE_HISTORY) */
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1056" id="l1056">  1056</a>     hist_add((CHAR *)p);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1057" id="l1057">  1057</a> }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1058" id="l1058">  1058</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1059" id="l1059">  1059</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1060" id="l1060">  1060</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1061" id="l1061">  1061</a> beg_line()
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1062" id="l1062">  1062</a> {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1063" id="l1063">  1063</a>     if (Point) {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1064" id="l1064">  1064</a> 	Point = 0;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1065" id="l1065">  1065</a> 	return CSmove;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1066" id="l1066">  1066</a>     }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1067" id="l1067">  1067</a>     return CSstay;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1068" id="l1068">  1068</a> }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1069" id="l1069">  1069</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1070" id="l1070">  1070</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1071" id="l1071">  1071</a> del_char()
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1072" id="l1072">  1072</a> {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1073" id="l1073">  1073</a>     return delete_string(Repeat == NO_ARG ? 1 : Repeat);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1074" id="l1074">  1074</a> }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1075" id="l1075">  1075</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1076" id="l1076">  1076</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1077" id="l1077">  1077</a> end_line()
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1078" id="l1078">  1078</a> {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1079" id="l1079">  1079</a>     if (Point != End) {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1080" id="l1080">  1080</a> 	Point = End;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1081" id="l1081">  1081</a> 	return CSmove;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1082" id="l1082">  1082</a>     }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1083" id="l1083">  1083</a>     return CSstay;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1084" id="l1084">  1084</a> }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1085" id="l1085">  1085</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1086" id="l1086">  1086</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1087" id="l1087">  1087</a> accept_line()
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1088" id="l1088">  1088</a> {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1089" id="l1089">  1089</a>     Line[End] = '\0';
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1090" id="l1090">  1090</a>     return CSdone;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1091" id="l1091">  1091</a> }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1092" id="l1092">  1092</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1093" id="l1093">  1093</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1094" id="l1094">  1094</a> transpose()
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1095" id="l1095">  1095</a> {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1096" id="l1096">  1096</a>     CHAR	c;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1097" id="l1097">  1097</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1098" id="l1098">  1098</a>     if (Point) {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1099" id="l1099">  1099</a> 	if (Point == End)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1100" id="l1100">  1100</a> 	    left(CSmove);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1101" id="l1101">  1101</a> 	c = Line[Point - 1];
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1102" id="l1102">  1102</a> 	left(CSstay);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1103" id="l1103">  1103</a> 	Line[Point - 1] = Line[Point];
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1104" id="l1104">  1104</a> 	TTYshow(Line[Point - 1]);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1105" id="l1105">  1105</a> 	Line[Point++] = c;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1106" id="l1106">  1106</a> 	TTYshow(c);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1107" id="l1107">  1107</a>     }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1108" id="l1108">  1108</a>     return CSstay;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1109" id="l1109">  1109</a> }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1110" id="l1110">  1110</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1111" id="l1111">  1111</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1112" id="l1112">  1112</a> quote()
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1113" id="l1113">  1113</a> {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1114" id="l1114">  1114</a>     unsigned int	c;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1115" id="l1115">  1115</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1116" id="l1116">  1116</a>     return (c = TTYget()) == EOF ? CSeof : insert_char((int)c);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1117" id="l1117">  1117</a> }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1118" id="l1118">  1118</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1119" id="l1119">  1119</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1120" id="l1120">  1120</a> wipe()
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1121" id="l1121">  1121</a> {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1122" id="l1122">  1122</a>     int		i;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1123" id="l1123">  1123</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1124" id="l1124">  1124</a>     if (Mark &gt; End)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1125" id="l1125">  1125</a> 	return ring_bell();
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1126" id="l1126">  1126</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1127" id="l1127">  1127</a>     if (Point &gt; Mark) {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1128" id="l1128">  1128</a> 	i = Point;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1129" id="l1129">  1129</a> 	Point = Mark;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1130" id="l1130">  1130</a> 	Mark = i;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1131" id="l1131">  1131</a> 	reposition();
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1132" id="l1132">  1132</a>     }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1133" id="l1133">  1133</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1134" id="l1134">  1134</a>     return delete_string(Mark - Point);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1135" id="l1135">  1135</a> }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1136" id="l1136">  1136</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1137" id="l1137">  1137</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1138" id="l1138">  1138</a> mk_set()
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1139" id="l1139">  1139</a> {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1140" id="l1140">  1140</a>     Mark = Point;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1141" id="l1141">  1141</a>     return CSstay;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1142" id="l1142">  1142</a> }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1143" id="l1143">  1143</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1144" id="l1144">  1144</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1145" id="l1145">  1145</a> exchange()
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1146" id="l1146">  1146</a> {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1147" id="l1147">  1147</a>     unsigned int	c;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1148" id="l1148">  1148</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1149" id="l1149">  1149</a>     if ((c = TTYget()) != CTL('X'))
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1150" id="l1150">  1150</a> 	return c == EOF ? CSeof : ring_bell();
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1151" id="l1151">  1151</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1152" id="l1152">  1152</a>     if ((c = Mark) &lt;= End) {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1153" id="l1153">  1153</a> 	Mark = Point;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1154" id="l1154">  1154</a> 	Point = c;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1155" id="l1155">  1155</a> 	return CSmove;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1156" id="l1156">  1156</a>     }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1157" id="l1157">  1157</a>     return CSstay;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1158" id="l1158">  1158</a> }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1159" id="l1159">  1159</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1160" id="l1160">  1160</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1161" id="l1161">  1161</a> yank()
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1162" id="l1162">  1162</a> {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1163" id="l1163">  1163</a>     if (Yanked &amp;&amp; *Yanked)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1164" id="l1164">  1164</a> 	return insert_string(Yanked);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1165" id="l1165">  1165</a>     return CSstay;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1166" id="l1166">  1166</a> }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1167" id="l1167">  1167</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1168" id="l1168">  1168</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1169" id="l1169">  1169</a> copy_region()
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1170" id="l1170">  1170</a> {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1171" id="l1171">  1171</a>     if (Mark &gt; End)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1172" id="l1172">  1172</a> 	return ring_bell();
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1173" id="l1173">  1173</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1174" id="l1174">  1174</a>     if (Point &gt; Mark)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1175" id="l1175">  1175</a> 	save_yank(Mark, Point - Mark);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1176" id="l1176">  1176</a>     else
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1177" id="l1177">  1177</a> 	save_yank(Point, Mark - Point);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1178" id="l1178">  1178</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1179" id="l1179">  1179</a>     return CSstay;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1180" id="l1180">  1180</a> }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1181" id="l1181">  1181</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1182" id="l1182">  1182</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1183" id="l1183">  1183</a> move_to_char()
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1184" id="l1184">  1184</a> {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1185" id="l1185">  1185</a>     unsigned int	c;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1186" id="l1186">  1186</a>     int			i;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1187" id="l1187">  1187</a>     CHAR		*p;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1188" id="l1188">  1188</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1189" id="l1189">  1189</a>     if ((c = TTYget()) == EOF)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1190" id="l1190">  1190</a> 	return CSeof;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1191" id="l1191">  1191</a>     for (i = Point + 1, p = &amp;Line[i]; i &lt; End; i++, p++)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1192" id="l1192">  1192</a> 	if (*p == c) {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1193" id="l1193">  1193</a> 	    Point = i;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1194" id="l1194">  1194</a> 	    return CSmove;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1195" id="l1195">  1195</a> 	}
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1196" id="l1196">  1196</a>     return CSstay;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1197" id="l1197">  1197</a> }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1198" id="l1198">  1198</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1199" id="l1199">  1199</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1200" id="l1200">  1200</a> fd_word()
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1201" id="l1201">  1201</a> {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1202" id="l1202">  1202</a>     return do_forward(CSmove);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1203" id="l1203">  1203</a> }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1204" id="l1204">  1204</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1205" id="l1205">  1205</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1206" id="l1206">  1206</a> fd_kill_word()
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1207" id="l1207">  1207</a> {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1208" id="l1208">  1208</a>     int		i;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1209" id="l1209">  1209</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1210" id="l1210">  1210</a>     (void)do_forward(CSstay);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1211" id="l1211">  1211</a>     if (OldPoint != Point) {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1212" id="l1212">  1212</a> 	i = Point - OldPoint;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1213" id="l1213">  1213</a> 	Point = OldPoint;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1214" id="l1214">  1214</a> 	return delete_string(i);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1215" id="l1215">  1215</a>     }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1216" id="l1216">  1216</a>     return CSstay;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1217" id="l1217">  1217</a> }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1218" id="l1218">  1218</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1219" id="l1219">  1219</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1220" id="l1220">  1220</a> bk_word()
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1221" id="l1221">  1221</a> {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1222" id="l1222">  1222</a>     int		i;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1223" id="l1223">  1223</a>     CHAR	*p;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1224" id="l1224">  1224</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1225" id="l1225">  1225</a>     i = 0;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1226" id="l1226">  1226</a>     do {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1227" id="l1227">  1227</a> 	for (p = &amp;Line[Point]; p &gt; Line &amp;&amp; !isalnum(p[-1]); p--)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1228" id="l1228">  1228</a> 	    left(CSmove);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1229" id="l1229">  1229</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1230" id="l1230">  1230</a> 	for (; p &gt; Line &amp;&amp; p[-1] != ' ' &amp;&amp; isalnum(p[-1]); p--)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1231" id="l1231">  1231</a> 	    left(CSmove);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1232" id="l1232">  1232</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1233" id="l1233">  1233</a> 	if (Point == 0)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1234" id="l1234">  1234</a> 	    break;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1235" id="l1235">  1235</a>     } while (++i &lt; Repeat);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1236" id="l1236">  1236</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1237" id="l1237">  1237</a>     return CSstay;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1238" id="l1238">  1238</a> }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1239" id="l1239">  1239</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1240" id="l1240">  1240</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1241" id="l1241">  1241</a> bk_kill_word()
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1242" id="l1242">  1242</a> {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1243" id="l1243">  1243</a>     (void)bk_word();
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1244" id="l1244">  1244</a>     if (OldPoint != Point)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1245" id="l1245">  1245</a> 	return delete_string(OldPoint - Point);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1246" id="l1246">  1246</a>     return CSstay;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1247" id="l1247">  1247</a> }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1248" id="l1248">  1248</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1249" id="l1249">  1249</a> STATIC int
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1250" id="l1250">  1250</a> argify(line, avp)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1251" id="l1251">  1251</a>     CHAR	*line;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1252" id="l1252">  1252</a>     CHAR	***avp;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1253" id="l1253">  1253</a> {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1254" id="l1254">  1254</a>     CHAR	*c;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1255" id="l1255">  1255</a>     CHAR	**p;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1256" id="l1256">  1256</a>     CHAR	**new;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1257" id="l1257">  1257</a>     int		ac;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1258" id="l1258">  1258</a>     int		i;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1259" id="l1259">  1259</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1260" id="l1260">  1260</a>     i = MEM_INC;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1261" id="l1261">  1261</a>     if ((*avp = p = NEW(CHAR*, i))== NULL)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1262" id="l1262">  1262</a> 	 return 0;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1263" id="l1263">  1263</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1264" id="l1264">  1264</a>     for (c = line; isspace(*c); c++)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1265" id="l1265">  1265</a> 	continue;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1266" id="l1266">  1266</a>     if (*c == '\n' || *c == '\0')
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1267" id="l1267">  1267</a> 	return 0;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1268" id="l1268">  1268</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1269" id="l1269">  1269</a>     for (ac = 0, p[ac++] = c; *c &amp;&amp; *c != '\n'; ) {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1270" id="l1270">  1270</a> 	if (isspace(*c)) {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1271" id="l1271">  1271</a> 	    *c++ = '\0';
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1272" id="l1272">  1272</a> 	    if (*c &amp;&amp; *c != '\n') {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1273" id="l1273">  1273</a> 		if (ac + 1 == i) {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1274" id="l1274">  1274</a> 		    new = NEW(CHAR*, i + MEM_INC);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1275" id="l1275">  1275</a> 		    if (new == NULL) {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1276" id="l1276">  1276</a> 			p[ac] = NULL;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1277" id="l1277">  1277</a> 			return ac;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1278" id="l1278">  1278</a> 		    }
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1279" id="l1279">  1279</a> 		    COPYFROMTO(new, p, i * sizeof (char **));
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1280" id="l1280">  1280</a> 		    i += MEM_INC;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1281" id="l1281">  1281</a> 		    DISPOSE(p);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1282" id="l1282">  1282</a> 		    *avp = p = new;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1283" id="l1283">  1283</a> 		}
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1284" id="l1284">  1284</a> 		p[ac++] = c;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1285" id="l1285">  1285</a> 	    }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1286" id="l1286">  1286</a> 	}
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1287" id="l1287">  1287</a> 	else
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1288" id="l1288">  1288</a> 	    c++;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1289" id="l1289">  1289</a>     }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1290" id="l1290">  1290</a>     *c = '\0';
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1291" id="l1291">  1291</a>     p[ac] = NULL;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1292" id="l1292">  1292</a>     return ac;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1293" id="l1293">  1293</a> }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1294" id="l1294">  1294</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1295" id="l1295">  1295</a> STATIC STATUS
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1296" id="l1296">  1296</a> last_argument()
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1297" id="l1297">  1297</a> {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1298" id="l1298">  1298</a>     CHAR	**av;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1299" id="l1299">  1299</a>     CHAR	*p;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1300" id="l1300">  1300</a>     STATUS	s;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1301" id="l1301">  1301</a>     int		ac;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1302" id="l1302">  1302</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1303" id="l1303">  1303</a>     if (H.Size == 1 || (p = H.Lines[H.Size - 2]) == NULL)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1304" id="l1304">  1304</a> 	return ring_bell();
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1305" id="l1305">  1305</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1306" id="l1306">  1306</a>     if ((p = (CHAR *)strdup((char *)p)) == NULL)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1307" id="l1307">  1307</a> 	return CSstay;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1308" id="l1308">  1308</a>     ac = argify(p, &amp;av);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1309" id="l1309">  1309</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1310" id="l1310">  1310</a>     if (Repeat != NO_ARG)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1311" id="l1311">  1311</a> 	s = Repeat &lt; ac ? insert_string(av[Repeat]) : ring_bell();
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1312" id="l1312">  1312</a>     else
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1313" id="l1313">  1313</a> 	s = ac ? insert_string(av[ac - 1]) : CSstay;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1314" id="l1314">  1314</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1315" id="l1315">  1315</a>     if (ac)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1316" id="l1316">  1316</a> 	DISPOSE(av);
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1317" id="l1317">  1317</a>     DISPOSE(p);
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1318" id="l1318">  1318</a>     return s;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1319" id="l1319">  1319</a> }
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1320" id="l1320">  1320</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1321" id="l1321">  1321</a> STATIC KEYMAP	Map[32] = {
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1322" id="l1322">  1322</a>     {	CTL('@'),	ring_bell	},
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1323" id="l1323">  1323</a>     {	CTL('A'),	beg_line	},
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1324" id="l1324">  1324</a>     {	CTL('B'),	bk_char		},
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1325" id="l1325">  1325</a>     {	CTL('D'),	del_char	},
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1326" id="l1326">  1326</a>     {	CTL('E'),	end_line	},
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1327" id="l1327">  1327</a>     {	CTL('F'),	fd_char		},
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1328" id="l1328">  1328</a>     {	CTL('G'),	ring_bell	},
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1329" id="l1329">  1329</a>     {	CTL('H'),	bk_del_char	},
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1330" id="l1330">  1330</a>     {	CTL('J'),	accept_line	},
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1331" id="l1331">  1331</a>     {	CTL('K'),	kill_line	},
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1332" id="l1332">  1332</a>     {	CTL('L'),	redisplay	},
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1333" id="l1333">  1333</a>     {	CTL('M'),	accept_line	},
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1334" id="l1334">  1334</a>     {	CTL('N'),	h_next		},
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1335" id="l1335">  1335</a>     {	CTL('O'),	ring_bell	},
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1336" id="l1336">  1336</a>     {	CTL('P'),	h_prev		},
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1337" id="l1337">  1337</a>     {	CTL('Q'),	ring_bell	},
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1338" id="l1338">  1338</a>     {	CTL('R'),	h_search	},
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1339" id="l1339">  1339</a>     {	CTL('S'),	ring_bell	},
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1340" id="l1340">  1340</a>     {	CTL('T'),	transpose	},
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1341" id="l1341">  1341</a>     {	CTL('U'),	ring_bell	},
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1342" id="l1342">  1342</a>     {	CTL('V'),	quote		},
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1343" id="l1343">  1343</a>     {	CTL('W'),	wipe		},
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1344" id="l1344">  1344</a>     {	CTL('X'),	exchange	},
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1345" id="l1345">  1345</a>     {	CTL('Y'),	yank		},
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1346" id="l1346">  1346</a>     {	CTL('Z'),	ring_bell	},
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1347" id="l1347">  1347</a>     {	CTL('['),	meta		},
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1348" id="l1348">  1348</a>     {	CTL(']'),	move_to_char	},
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1349" id="l1349">  1349</a>     {	CTL('^'),	ring_bell	},
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1350" id="l1350">  1350</a>     {	CTL('_'),	ring_bell	},
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1351" id="l1351">  1351</a>     {	0,		NULL		}
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1352" id="l1352">  1352</a> };
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1353" id="l1353">  1353</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1354" id="l1354">  1354</a> STATIC KEYMAP	MetaMap[16]= {
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1355" id="l1355">  1355</a>     {	CTL('H'),	bk_kill_word	},
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1356" id="l1356">  1356</a>     {	DEL,		bk_kill_word	},
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1357" id="l1357">  1357</a>     {	' ',		mk_set		},
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1358" id="l1358">  1358</a>     {	'.',		last_argument	},
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1359" id="l1359">  1359</a>     {	'&lt;',		h_first		},
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1360" id="l1360">  1360</a>     {	'&gt;',		h_last		},
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1361" id="l1361">  1361</a>     {	'b',		bk_word		},
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1362" id="l1362">  1362</a>     {	'd',		fd_kill_word	},
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1363" id="l1363">  1363</a>     {	'f',		fd_word		},
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1364" id="l1364">  1364</a>     {	'l',		case_down_word	},
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1365" id="l1365">  1365</a>     {	'm',		toggle_meta_mode },
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1366" id="l1366">  1366</a>     {	'u',		case_up_word	},
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1367" id="l1367">  1367</a>     {	'y',		yank		},
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1368" id="l1368">  1368</a>     {	'w',		copy_region	},
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1369" id="l1369">  1369</a>     {	0,		NULL		}
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l1370" id="l1370">  1370</a> };
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l1371" id="l1371">  1371</a> 
</pre></div>
</div>

<div class="page_footer">
<div class="page_footer_text">mozilla-central</div>
<div class="feed">
<a href="/mozilla-central/atom-log"><img src="http://www.mozilla.org/images/livemarks16.png" alt="Feed" title="Feed of repository changes"/></a>
</div>
<br />

</div>
</body>
</html>

