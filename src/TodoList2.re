module TodoItem = Fragments.TodoItem;
module TodoListFragment = [%graphql
  {|
    query TodoList_query($first: Int, $after: String) {
      todosConnection(first: $first, after: $after) {
        edges {
          node {
            id
            ...TodoItem
          }
        }
      }
    }
|}
];

module AddTodoMutation = [%graphql
  {|
mutation TodoListAddTodoMutation($input: AddTodoItemInput!) {
  addTodoItem(input: $input) {
    addedTodoItemEdge {
      node {
        id
        text
        completed
      }
    }
  }
}
|}
];

[@react.component]
let make = (~query as queryRef) => {
  let todoListData = TodoListFragment.use(queryRef);
  let (addTodo, addTodoResult) = AddTodoMutation.use();

  let (newTodoText, setNewTodoText) = React.useState(() => "");
  // Error: The function applied to this argument has type
  //          (~input: AddTodoMutation.t_variables_AddTodoItemInput,
  //          ~connections: array(string)) =>
  //          AddTodoMutation.AddTodoMutation_inner.t_variables
  <div className="card">
    <div className="card-body">
      <h4 className="card-title text-white"> {React.string("Todo")} </h4>
      <form
        onSubmit={ev => {
          ReactEvent.Form.preventDefault(ev);
          let input: AddTodoMutation.t_variables = {
            input: {
              text: newTodoText,
              clientMutationId: None,
            },
          };
          let input =
            AddTodoMutation.makeVariables(
              ~input={text: newTodoText, clientMutationId: None},
              (),
            );

          addTodo(
            // ~onCompleted=(() => {setNewTodoText(_ => "")},
            ~optimisticResponse=
              _variables =>
                {
                  addTodoItem:
                    Some({
                      addedTodoItemEdge:
                        Some({
                          node:
                            Some({
                              __typename: "placeholder",
                              id: "placeholder",
                              text: newTodoText,
                              completed: Some(false),
                            }),
                          __typename: "placeholder",
                        }),
                      __typename: "placeholder",
                    }),
                },
            input,
          )
          ->ignore;
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
            disabled={
              switch (newTodoText) {
              | "" => true
              | _ => false
              }
            }>
            {React.string("Add")}
          </button>
        </div>
      </form>
      <div className="list-wrapper">

          <ul className="d-flex flex-column-reverse todo-list todo-list-custom">
            {switch (todoListData) {
             | {data: Some(todosConnection)} =>
               Js.log2("todosConnection", todosConnection);
               let edges = Js.Nullable.return(todosConnection);
               Js.log2("todosConnection:edges", edges);
               //      <SingleTodo
               //        key={todoItem.id}
               //        todoItem={todoItem.fragmentRefs}
               //        checked=true
               //      />
               //    )
               //  ->React.array}
               React.null;
             // }}
             | {data: None} => React.null
             }}
          </ul>
        </div>
        // {todoListData.todosConnection
        // //  ->TodoListFragment.addedTodoItemEdge
        //  ->Belt.Array.map(todoItem =>
        //      <SingleTodo
        //        key={todoItem.id}
        //        todoItem={todoItem.fragmentRefs}
        //        checked=true
        //      />
        //    )
        //  ->React.array}
    </div>
  </div>;
};
