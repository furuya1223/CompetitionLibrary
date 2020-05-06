---
layout: default
---

<!-- mathjax config similar to math.stackexchange -->
<script type="text/javascript" async
  src="https://cdnjs.cloudflare.com/ajax/libs/mathjax/2.7.5/MathJax.js?config=TeX-MML-AM_CHTML">
</script>
<script type="text/x-mathjax-config">
  MathJax.Hub.Config({
    TeX: { equationNumbers: { autoNumber: "AMS" }},
    tex2jax: {
      inlineMath: [ ['$','$'] ],
      processEscapes: true
    },
    "HTML-CSS": { matchFontHeight: false },
    displayAlign: "left",
    displayIndent: "2em"
  });
</script>

<script type="text/javascript" src="https://cdnjs.cloudflare.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
<script src="https://cdn.jsdelivr.net/npm/jquery-balloon-js@1.1.2/jquery.balloon.min.js" integrity="sha256-ZEYs9VrgAeNuPvs15E39OsyOJaIkXEEt10fzxJ20+2I=" crossorigin="anonymous"></script>
<script type="text/javascript" src="assets/js/copy-button.js"></script>
<link rel="stylesheet" href="assets/css/copy-button.css" />


# {{ site.title }}

[![Actions Status]({{ site.github.repository_url }}/workflows/verify/badge.svg)]({{ site.github.repository_url }}/actions)
<a href="{{ site.github.repository_url }}"><img src="https://img.shields.io/github/last-commit/{{ site.github.owner_name }}/{{ site.github.repository_name }}" /></a>

{% if site.github.project_tagline %}{{ site.github.project_tagline }}{% else %}This documentation is automatically generated by <a href="https://github.com/kmyk/online-judge-verify-helper">online-judge-verify-helper</a>.{% endif %}

## Library Files

<div id="d521f765a49c72507257a2620612ee96"></div>

### library

* :warning: <a href="library/library/aug_dijkstra.hpp.html">library/aug_dijkstra.hpp</a>
* :warning: <a href="library/library/baranced_binary_tree.hpp.html">library/baranced_binary_tree.hpp</a>
* :warning: <a href="library/library/convex_hull_trick.hpp.html">library/convex_hull_trick.hpp</a>
* :warning: <a href="library/library/extgcd_garner.hpp.html">library/extgcd_garner.hpp</a>
* :warning: <a href="library/library/fft.hpp.html">library/fft.hpp</a>
* :warning: <a href="library/library/geometry.hpp.html">library/geometry.hpp</a>
* :warning: <a href="library/library/graph.hpp.html">library/graph.hpp</a>
* :heavy_check_mark: <a href="library/library/header.hpp.html">library/header.hpp</a>
* :warning: <a href="library/library/hl_decomposition.hpp.html">library/hl_decomposition.hpp</a>
* :warning: <a href="library/library/library.hpp.html">library/library.hpp</a>
* :warning: <a href="library/library/low_link.hpp.html">library/low_link.hpp</a>
* :warning: <a href="library/library/lowest_common_ancestor.hpp.html">library/lowest_common_ancestor.hpp</a>
* :warning: <a href="library/library/matrix.hpp.html">library/matrix.hpp</a>
* :heavy_check_mark: <a href="library/library/max_flow.hpp.html">library/max_flow.hpp</a>
* :heavy_check_mark: <a href="library/library/min_cost_flow.hpp.html">library/min_cost_flow.hpp</a>
* :warning: <a href="library/library/mod_int.hpp.html">library/mod_int.hpp</a>
* :heavy_check_mark: <a href="library/library/ntt.hpp.html">library/ntt.hpp</a>
* :warning: <a href="library/library/rational.hpp.html">library/rational.hpp</a>
* :heavy_check_mark: <a href="library/library/segment_tree.hpp.html">library/segment_tree.hpp</a>
* :heavy_check_mark: <a href="library/library/shortest_path.hpp.html">library/shortest_path.hpp</a>
* :warning: <a href="library/library/string_algorithm.hpp.html">library/string_algorithm.hpp</a>
* :heavy_check_mark: <a href="library/library/strongly_connected_component.hpp.html">library/strongly_connected_component.hpp</a>
* :warning: <a href="library/library/suffix_array.hpp.html">library/suffix_array.hpp</a>
* :heavy_check_mark: <a href="library/library/union_find.hpp.html">library/union_find.hpp</a>
* :warning: <a href="library/library/wavelet_matrix.hpp.html">library/wavelet_matrix.hpp</a>


## Verify Files

* :heavy_check_mark: <a href="verify/test/dijkstra.test.cpp.html">test/dijkstra.test.cpp</a>
* :heavy_check_mark: <a href="verify/test/max_flow_dinic.test.cpp.html">test/max_flow_dinic.test.cpp</a>
* :heavy_check_mark: <a href="verify/test/min_cost_flow.test.cpp.html">test/min_cost_flow.test.cpp</a>
* :heavy_check_mark: <a href="verify/test/ntt.test.cpp.html">test/ntt.test.cpp</a>
* :heavy_check_mark: <a href="verify/test/segment_tree_RMQ.test.cpp.html">test/segment_tree_RMQ.test.cpp</a>
* :heavy_check_mark: <a href="verify/test/segment_tree_RSQ.test.cpp.html">test/segment_tree_RSQ.test.cpp</a>
* :heavy_check_mark: <a href="verify/test/strongly_connected_component.test.cpp.html">test/strongly_connected_component.test.cpp</a>
* :heavy_check_mark: <a href="verify/test/union_find.test.cpp.html">test/union_find.test.cpp</a>

