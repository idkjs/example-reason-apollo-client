/**
 * This mounts the app to the DOM. Note that the environment is
 * passed to context via <ReasonRelay.Context.Provider /> here -
 * it's a requirement that the environment is available in the
 * context.
 */
ReactDOMRe.renderToElementWithId(
  <ApolloClient.React.ApolloProvider client=Client.instance>
    <App />
  </ApolloClient.React.ApolloProvider>,
  "app",
);
// ReactExperimental.renderConcurrentRootAtElementWithId(
//   <ApolloClient.React.ApolloProvider client=Client.instance>
//     <App />
//   </ApolloClient.React.ApolloProvider>,
//   "app",
// );