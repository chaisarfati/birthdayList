
function showForm(){
  var but = document.getElementById('showSearchBar');

  var textField = document.createElement('input');
  textField.id = 'searchField';
  textField.name = 'q';
  textField.type = "textfield";
  textField.placeholder = "search list number, name...";

  var submit = document.createElement('input');
  submit.type = 'submit';
  submit.value = "Search";
  submit.id = "search_butt";

  var form = document.createElement('form');
  form.appendChild(textField);
  form.appendChild(submit);

  document.getElementById('search').replaceChild(form, but);
}

function mouseOnSub(){
  document.getElementById("submit_butt").value="Validate the entries >>";
}

function mouseOutSub(){
  document.getElementById("submit_butt").value="Validate the entries";
}
