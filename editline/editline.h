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

<title>mozilla-central: js/src/editline/editline.h@b2fd9044b65c</title>
<link rel="alternate" type="application/atom+xml"
   href="/mozilla-central/atom-log" title="Atom feed for mozilla-central"/>
</head>
<body>

<div class="page_header">
<a href="http://developer.mozilla.org/en/docs/Mercurial" title="Mercurial" style="float: right;">Mercurial</a><a href="/mozilla-central/summary">mozilla-central</a> - file revision - js/src/editline/editline.h@b2fd9044b65c
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
<a href="/mozilla-central/log/b2fd9044b65c/js/src/editline/editline.h">revisions</a> |
<a href="/mozilla-central/annotate/b2fd9044b65c/js/src/editline/editline.h">annotate</a> |
<a href="/mozilla-central/diff/b2fd9044b65c/js/src/editline/editline.h">diff</a> |
<a href="/mozilla-central/raw-file/b2fd9044b65c/js/src/editline/editline.h">raw</a><br/>
</div>

<div class="title">js/src/editline/editline.h</div>

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
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l60" id="l60">    60</a> /*
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l61" id="l61">    61</a> **  Internal header file for editline library.
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l62" id="l62">    62</a> */
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l63" id="l63">    63</a> #include &lt;stdio.h&gt;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l64" id="l64">    64</a> #if	defined(HAVE_STDLIB)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l65" id="l65">    65</a> #include &lt;stdlib.h&gt;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l66" id="l66">    66</a> #include &lt;string.h&gt;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l67" id="l67">    67</a> #endif	/* defined(HAVE_STDLIB) */
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l68" id="l68">    68</a> #if	defined(SYS_UNIX)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l69" id="l69">    69</a> #include &quot;unix.h&quot;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l70" id="l70">    70</a> #endif	/* defined(SYS_UNIX) */
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l71" id="l71">    71</a> #if	defined(SYS_OS9)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l72" id="l72">    72</a> #include &quot;os9.h&quot;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l73" id="l73">    73</a> #endif	/* defined(SYS_OS9) */
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l74" id="l74">    74</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l75" id="l75">    75</a> #if	!defined(SIZE_T)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l76" id="l76">    76</a> #define SIZE_T	unsigned int
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l77" id="l77">    77</a> #endif	/* !defined(SIZE_T) */
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l78" id="l78">    78</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l79" id="l79">    79</a> typedef unsigned char	CHAR;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l80" id="l80">    80</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l81" id="l81">    81</a> #if	defined(HIDE)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l82" id="l82">    82</a> #define STATIC	static
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l83" id="l83">    83</a> #else
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l84" id="l84">    84</a> #define STATIC	/* NULL */
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l85" id="l85">    85</a> #endif	/* !defined(HIDE) */
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l86" id="l86">    86</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l87" id="l87">    87</a> #if	!defined(CONST)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l88" id="l88">    88</a> #if	defined(__STDC__)
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l89" id="l89">    89</a> #define CONST	const
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l90" id="l90">    90</a> #else
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l91" id="l91">    91</a> #define CONST
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l92" id="l92">    92</a> #endif	/* defined(__STDC__) */
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l93" id="l93">    93</a> #endif	/* !defined(CONST) */
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l94" id="l94">    94</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l95" id="l95">    95</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l96" id="l96">    96</a> #define MEM_INC		64
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l97" id="l97">    97</a> #define SCREEN_INC	256
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l98" id="l98">    98</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l99" id="l99">    99</a> #define DISPOSE(p)	free((char *)(p))
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l100" id="l100">   100</a> #define NEW(T, c)	\
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l101" id="l101">   101</a> 	((T *)malloc((unsigned int)(sizeof (T) * (c))))
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l102" id="l102">   102</a> #define RENEW(p, T, c)	\
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l103" id="l103">   103</a> 	(p = (T *)realloc((char *)(p), (unsigned int)(sizeof (T) * (c))))
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l104" id="l104">   104</a> #define COPYFROMTO(new, p, len)	\
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l105" id="l105">   105</a> 	(void)memcpy((char *)(new), (char *)(p), (int)(len))
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l106" id="l106">   106</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l107" id="l107">   107</a> 
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l108" id="l108">   108</a> /*
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l109" id="l109">   109</a> **  Variables and routines internal to this package.
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l110" id="l110">   110</a> */
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l111" id="l111">   111</a> extern int	rl_eof;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l112" id="l112">   112</a> extern int	rl_erase;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l113" id="l113">   113</a> extern int	rl_intr;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l114" id="l114">   114</a> extern int	rl_kill;
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l115" id="l115">   115</a> extern int	rl_quit;
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l116" id="l116">   116</a> extern char	*rl_complete();
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l117" id="l117">   117</a> extern int	rl_list_possib();
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l118" id="l118">   118</a> extern void	rl_ttyset();
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l119" id="l119">   119</a> extern void	rl_add_slash();
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l120" id="l120">   120</a> 
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l121" id="l121">   121</a> #if	!defined(HAVE_STDLIB)
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l122" id="l122">   122</a> extern char	*getenv();
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l123" id="l123">   123</a> extern char	*malloc();
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l124" id="l124">   124</a> extern char	*realloc();
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l125" id="l125">   125</a> extern char	*memcpy();
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l126" id="l126">   126</a> extern char	*strcat();
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l127" id="l127">   127</a> extern char	*strchr();
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l128" id="l128">   128</a> extern char	*strrchr();
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l129" id="l129">   129</a> extern char	*strcpy();
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l130" id="l130">   130</a> extern char	*strdup();
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l131" id="l131">   131</a> extern int	strcmp();
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l132" id="l132">   132</a> extern int	strlen();
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l133" id="l133">   133</a> extern int	strncmp();
</pre></div><div style="font-family:monospace" class="parity1"><pre><a class="linenr" href="#l134" id="l134">   134</a> #endif	/* !defined(HAVE_STDLIB) */
</pre></div><div style="font-family:monospace" class="parity0"><pre><a class="linenr" href="#l135" id="l135">   135</a> 
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

