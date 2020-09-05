module QueryResult = ApolloClient.Types.QueryResult;
module TodoItem = Fragments.TodoItem;

// module TodoQuery =
[%graphql
  {|
    query TodoQuery($id:ID!) {
      todoItem:node(id: $id) {

        ...TodoItem
      }
    }
  |}
];

[@react.component]
let make = (~checked, ~todoItem as todoItemRef) => {
  switch (TodoQuery.use({id: todoItemRef})) {
  | {loading: true} => "Loading..."->React.string
  // | {error: Some(_error)} => "Error loading data"->React.string
  | {data: Some({todoItem: Some({todoItem})})} =>
    <div>
      <li
        className={
          switch (todoItem.completed) {
          | Some(true) => "completed"
          | Some(false)
          | None => ""
          }
        }>
        "There are "->React.string
        // {todoItem->Belt.Array.length->React.int}
        " To-Dos"->React.string
      </li>
    </div>

  | {loading: false, data: None, error} =>
    Js.log(error);
    <p> "Error loading data"->React.string </p>;
  | _ => <p> "Error loading data"->React.string </p>
  };
};
