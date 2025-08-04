lilystatic
===

A zero-dependency static site framework. Written in C. Easy and simple!

#### Directory-Based Rendering
```bash
 |-- pages
 |   |-- index.html
 |   |-- about.html
 |   |-- blog
 |       |-- post_1.md
 |       |-- post_2.md
 |
 |-- templates
 |   |-- root.html
 |   |-- blog.html
```

`templates/root.html`
```html
<html>
  <head>
    <title>TITLE</title>
    <body>
      <!-- INNER -->
    </body>
  </head>
</html>
```

`pages/index.html`
```html
<!-- ROOT -->
  <h1>Hello!</h1>
  <h2>It's a post</h2>
<!-- /ROOT -->
```

```html
<!-- BLOG -->
  <
<!-- /BLOG -->
```
