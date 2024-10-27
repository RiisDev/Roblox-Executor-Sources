import { StateDocument } from "@hydrogen-web/models";
import {
  createContext,
  Dispatch,
  FC,
  PropsWithChildren,
  SetStateAction,
  useState,
} from "react";

export const StateContext = createContext<{
  state: StateDocument | null;
  updateState: Dispatch<SetStateAction<StateDocument | null>>;
}>({
  state: null,
  updateState: () => {
    throw "Not loaded";
  },
});

export const StateContextProvider: FC<PropsWithChildren> = ({ children }) => {
  const [state, setState] = useState<StateDocument | null>(null);

  return (
    <StateContext.Provider value={{ state, updateState: setState }}>
      {children}
    </StateContext.Provider>
  );
};
