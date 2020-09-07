// open Types;

// module Fragment = [%graphql
//   {|
//   fragment TodoItem on TodoItem @bsRecord {
//     id
//     text
//     completed
//   }
// |}
// ];
module TodoItem = Fragments.TodoItem;
module UpdateTodoMutation = [%graphql
  {|
  mutation ($id: ID!, $text: String!, $completed: Boolean!) {
    updateTodoSimple(id: $id, text: $text, completed: $completed) {
      ...TodoItem
    }
  }
  |}
];

module DeleteTodoMutation = [%graphql
  {|
  mutation ($id: ID!) {
    deleteTodoSimple(id: $id) {
      deletedTodoItemId
    }
  }
|}
];

[@react.component]
let make = (~todo: TodoItem.t, ~refetchQueries) => {
  let (updateTodoItem, _) = UpdateTodoMutation.use();
  let (deleteTodoItem, _) = DeleteTodoMutation.use();
let todoCompleted = Belt.Option.getWithDefault(todo.completed, false);
  let handleUpdate = _ => {
    let completed = !Belt.Option.getWithDefault(todo.completed, false);
    let variables =
      UpdateTodoMutation.makeVariables(
        ~id=todo.id,
        ~text=todo.text,
        ~completed,
        (),
      );

    updateTodoItem(variables) |> ignore;
  };

  let handleDelete = _ => {
    deleteTodoItem(
      DeleteTodoMutation.makeVariables(~id=todo.id, ()),
      ~refetchQueries,
    )
    |> ignore;
  };

  <li className={todoCompleted ? "completed": ""}>
    <div className="form-check">
      <label className="form-check-label">
        <input
          className="checkbox"
          type_="checkbox"
          checked={todoCompleted}
          onChange=handleUpdate
        />
        {React.string(todo.text)}
      </label>
    </div>
    <i
      onClick=handleDelete
      role="button"
      className="remove mdi mdi-close-circle-outline"
    />
  </li>;
};
