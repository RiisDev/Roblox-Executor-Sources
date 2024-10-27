import { Button, Group, GroupProps } from "@mantine/core";
import { FC } from "react";

type Props = GroupProps;

export const AuthDialog: FC<Props> = ({ ...rest }) => {
  const user = false;

  return user ? (
    <Button
      size="xs"
      component="a"
      href={`dashboard.${process.env.NEXT_PUBLIC_HOSTNAME}`}
      variant="gradient"
      gradient={
        {
          deg: 20,
          to: "#76a4cf",
        } as any
      }
    >
      Dashboard
    </Button>
  ) : (
    <Group spacing="xs" {...rest}>
      <Button
        color="gray"
        size="xs"
        variant="outline"
        component="a"
        href={`dashboard.${process.env.NEXT_PUBLIC_HOSTNAME}/sign-up`}
      >
        Sign Up
      </Button>
      <Button
        size="xs"
        variant="gradient"
        gradient={
          {
            deg: 20,
            to: "#76a4cf",
          } as any
        }
        component="a"
        href={`dashboard.${process.env.NEXT_PUBLIC_HOSTNAME}/log-in`}
      >
        Log In
      </Button>
    </Group>
  );
};
