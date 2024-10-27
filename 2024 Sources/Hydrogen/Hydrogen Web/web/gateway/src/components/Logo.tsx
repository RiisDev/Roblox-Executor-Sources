import { Group, MantineColor, Title } from "@mantine/core";
import { IconDroplet } from "@tabler/icons";
import Link from "next/link";
import { FC } from "react";

type Props = {
  color?: MantineColor;
};

export const Logo: FC<Props> = ({ color }) => {
  return (
    <Link href={`https://${process.env.NEXT_PUBLIC_HOSTNAME}`}>
      <Group
        spacing={2}
        sx={{
          cursor: "pointer",
          userSelect: "none",
        }}
      >
        <IconDroplet />
        <Title
          sx={{
            color: color || undefined,
            fontWeight: 500,
            fontSize: 20,
            whiteSpace: "nowrap",
          }}
        >
          Hydrogen
        </Title>
      </Group>
    </Link>
  );
};
