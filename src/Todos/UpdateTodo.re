open Types;

// module TodoItem = TodoFragments.TodoItem;
module TodoItem = Fragments.TodoItem;

// module TodosQuery = [%graphql
//   {|
//     query TodosQuery {
//       todos: allTodos {
//         # This references the TodoItem fragment definition module above!
//         ...TodoItem
//       }
//     }
//   |}
// ];
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
let make = (~todo: todo, ~refetchQueries) => {
  let (updateTodoItem, _) = UpdateTodoMutation.use();

  let (deleteTodoItem, _) = DeleteTodoMutation.use();

  let handleUpdate = _ => {
    let updateTodoSimpleVariables: UpdateTodoMutation.t = {
      updateTodoSimple:
        Some({
          __typename: todo.__typename,
          id: todo.id,
          text: todo.text,
          completed: Some(!todo.completed),
        }),
    };
    // next two same type but called differently? `variables` doesnt work when passed
    let variables: UpdateTodoMutation.t_variables =
      UpdateTodoMutation.makeVariables(
        ~id=todo.id,
        ~text=todo.text,
        ~completed=!todo.completed,
        (),
      );
    let updateTodoSimpleMutationTVariables: UpdateTodoMutation.t_variables = {
      id: todo.id,
      text: todo.text,
      completed: !todo.completed,
    };
    let updateTodoSimpleInnerVariables: UpdateTodoMutation.UpdateTodoMutation_inner.t_variables = {
      id: todo.id,
      text: todo.text,
      completed: !todo.completed,
    };

    // updateTodoItem({id: todo.id, text: todo.text, completed: !todo.completed})
    // |> ignore;
    // this doesnt work though same time
    // updateTodoItem(updateTodoSimpleMutationTVariables) |> ignore;
    updateTodoItem(updateTodoSimpleInnerVariables) |> ignore;
  };

  let handleDelete = _ => {
    deleteTodoItem(DeleteTodoMutation.{id: todo.id}, ~refetchQueries)
    |> ignore;
  };

  <li className={todo.completed ? "completed" : ""}>
    <div className="form-check">
      <label className="form-check-label">
        <input
          className="checkbox"
          type_="checkbox"
          checked={todo.completed}
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
