<?php
/// (C) Michael Moritz <miiimooo@drupal.org>
class child_remover_filter_handler extends views_handler_filter {
  function query() {
//     $table_alias = $this->query->ensure_table('node');
    $table_alias = $this->query->ensure_table('book');
    $table_alias_menu_links = $this->query->ensure_table('menu_links');
    $table_alias_book_menu_links = $this->query->ensure_table('book_menu_links');
    $join = new views_join;
    $join->construct('menu_links', 'book', 'mlid', 'mlid');
    $this->query->add_relationship('book_menu_links', $join, 'book_menu_links', 'book_menu_links');
    $where = "book_menu_links.depth = 1 OR book_menu_links.depth IS NULL";
    $this->query->add_where($this->options['group'], $where);
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['operator']['default'] = '';
    $options['value']['default'] = '';
    return $options;
  }
}
