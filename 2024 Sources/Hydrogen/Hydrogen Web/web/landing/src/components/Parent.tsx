import { FC, PropsWithChildren, ReactNode } from "react";

type Props = {
  parent?: (children: ReactNode) => ReactNode;
};

export const Parent: FC<PropsWithChildren<Props>> = ({ parent, children }) => (
  <>{parent ? parent(children) : children}</>
);
