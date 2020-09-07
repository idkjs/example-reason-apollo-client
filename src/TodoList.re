module TodoItem = Fragments.TodoItem;
module AllTodosQuery = [%graphql
  {|
  query TodosQuery {
    allTodos {
      ...TodoItem
    }
  }
|}
];

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

module AddTodoMutation = [%graphql
  {|
  mutation ($text: String!) {
    addTodoSimple(text: $text) {
      id
    }
  }
|}
];

[@react.component]
let make = () => {
  //   let (todosResult, _) = AllTodosQuery.use();
  let todosResult = AllTodosQuery.use();
  let (addTodo, addTodoResult) = AddTodoMutation.use();

  let (newTodoText, setNewTodoText) = React.useState(() => "");
  let canAddTodo = newTodoText != "" && !addTodoResult.loading;

  let refetchQueries = [|AllTodosQuery.refetchQueryDescription()|];
  let handleAddTodo = () => {
    let variables = AddTodoMutation.makeVariables(~text=newTodoText, ());

    addTodo(variables, ~refetchQueries)
    |> Js.Promise.(then_(_ => setNewTodoText(_ => "") |> resolve))
    |> ignore;
  };

  <div className="card">
    <div className="card-body">
      <h4 className="card-title text-white"> {React.string("Todo")} </h4>
      <form
        onSubmit={e => {
          ReactEvent.Form.preventDefault(e);
          handleAddTodo();
        }}>
        <div className="add-items d-flex">
          <input
            type_="text"
            className="form-control todo-list-input"
            placeholder="What do you need to do today?"
            value=newTodoText
            onChange={event => {
              let value = ReactEvent.Form.currentTarget(event)##value;
              setNewTodoText(_ => value);
            }}
          />
          <button
            type_="submit"
            className="add btn btn-gradient-primary font-weight-bold todo-list-add-btn"
            id="add-task"
            disabled={!canAddTodo}>
            {React.string("Add")}
          </button>
        </div>
      </form>
      <div className="list-wrapper">
        <ul className="d-flex flex-column-reverse todo-list todo-list-custom">
          {switch (todosResult) {
           | {loading: false, data: Some(data)} =>
             {
               data.allTodos
               ->Belt.Array.map(todo =>
                   <Todo key={todo.id} todo refetchQueries />
                 );
             }
             |> React.array
           | {loading: true} => <Spinner />
           | {error} =>
             <div>
               "Error loading data"->React.string
               {switch (error) {
                | Some(error) => React.string(": " ++ error.message)
                | None => React.null
                }}
             </div>
           }}
        </ul>
      </div>
    </div>
  </div>;
};
