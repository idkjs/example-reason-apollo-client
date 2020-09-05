module Cache = ApolloClient.Cache;

module DeleteTodoMutation = [%graphql
  {|
      # This has to be ID! and not String! or it will fail silently
    mutation DeleteTodo ($id:ID!){
      deleteTodoItem(input: { id: $id }) {
        deletedTodoItemId
        clientMutationId
      }
    }
  |}
];
module UpdateMutation = [%graphql
  {|
  mutation SingleTodoUpdateMutation($input: UpdateTodoItemInput!) {
    updateTodoItem(input: $input) {
      updatedTodoItem {
        id
        text
        completed
      }
    }
  }
|}
];
//  type t_updateTodoItem = {
//     __typename: string,
//     updatedTodoItem: option(t_updateTodoItem_updatedTodoItem),
//   };

// and t_variables_UpdateTodoItemInput = {
// id: string,
// text: string,
// completed: bool,
// clientMutationId: option(string),
// };
// type t_variables = { input: t_variables_UpdateTodoItemInput, }
// type t_updateTodoItem = {
//     __typename: string,
//     updatedTodoItem: option(t_updateTodoItem_updatedTodoItem),
//   };
//   type t = { updateTodoItem: option(t_updateTodoItem), };
[@react.component]
let make = (~id: string, ~todoItem as todoItemRef) => {
  let (_, setId) = React.useState(_ => id);

  let (mutate, result) = UpdateMutation.use();
  let x: UpdateMutation.UpdateMutation_inner.t_variables = {
    input: {
      id: "todoItemRef.id",
      completed:false,
      text: "todoItemRef.text",
      clientMutationId: None,
    },
  };
  let y: UpdateMutation.t = {
    // __typename: "string",
    updateTodoItem:
      Some({
        __typename: "dfa",
        updatedTodoItem:
          Some({
            __typename: "string",
            id: "string",
            text: "string",
            completed: Some(true),
            // clientMutationId: None,
          }),
      }),
  };
  let handleSubmit = event => {
    ReactEvent.Form.preventDefault(event);
    mutate({id: todoItemRef})->ignore;
  };
  switch (result) {
  | {called: false} =>
    <>
      "Not called... "->React.string
      <form onSubmit=handleSubmit>
        <h2> {React.string("Delete Id: " ++ id ++ "?")} </h2>
        <div className="form-field">
          <input
            placeholder=id
            value=id
            onChange={event => {
              let value = event->ReactEvent.Form.target##value;
              setId(value);
            }}
          />
        </div>
        <div className="form-field">
          <input type_="submit" value="Delete ToDo" />
        </div>
      </form>
    </>
  | {loading: true} => "Loading..."->React.string
  | {data: Some({deleteTodoItem}), error: None} =>
    let deletedTodoItemId =
      deleteTodoItem
      ->Belt.Option.map(d => d.deletedTodoItemId)
      ->Belt.Option.getWithDefault(Some(""));

    let deletedTodoItemId =
      switch (deletedTodoItemId) {
      | Some(id) => id
      | None => ""
      };
    <p>
      {React.string("To-Do id: \"" ++ deletedTodoItemId ++ "\" deleted")}
    </p>;
  | {error} =>
    <>
      "Error loading data"->React.string
      {switch (error) {
       | Some(error) => React.string(": " ++ error.message)
       | None => React.null
       }}
    </>
  };
};
