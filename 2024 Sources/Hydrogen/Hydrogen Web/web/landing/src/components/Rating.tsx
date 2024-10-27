import { Group } from "@mantine/core";
import { IconStar } from "@tabler/icons";
import { FC } from "react";

type Props = {
  rating: number;
};

export const Rating: FC<Props> = ({ rating }) => (
  <Group spacing={0}>
    {new Array(4).fill(null).map((_, i) => (
      <IconStar key={i} size={16} fill={rating > i ? "#fff" : "none"} />
    ))}
  </Group>
);
