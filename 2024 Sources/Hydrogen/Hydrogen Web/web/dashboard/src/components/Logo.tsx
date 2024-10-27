import { Group, MantineColor, Title } from "@mantine/core";
import { IconDroplet } from "@tabler/icons";
import { useRouter } from "next/router";
import { FC } from "react";

type Props = {
  color?: MantineColor;
};

export const Logo: FC<Props> = ({ color }) => {
  const router = useRouter();

  return (
    <Group spacing={2}>
      <IconDroplet />
      <Title
        sx={{
          color: color || undefined,
          fontWeight: 500,
          fontSize: 20,
          whiteSpace: "nowrap",
          cursor: "pointer",
          userSelect: "none",
        }}
        onClick={() => router.push("/")}
      >
        Hydrogen
      </Title>
    </Group>
  );
};
